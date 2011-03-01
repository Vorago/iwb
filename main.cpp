//#include <cstdio>
#include "include/capture.hpp"
#include "include/analysis.hpp"
#include "include/presentation.hpp"
#include "include/scroller.hpp"

using namespace iwb;

bool argHandler(int argc, char* argv[], Capture **cpt, int *resWidth, int* resHeight){
    int cam;

    // Argument handling block
    if (argc >= 2) {
        char *endptr;
        cam = strtol(argv[1], &endptr, 10);
        if (*endptr == '\0') {
            *cpt = new Capture(cam);
        } else {
            *cpt = new Capture(argv[1]);
        }
    } else {
        cam = 0;
        *cpt = new Capture(cam);
    }

    /**
     * Process resolution in second parameter.
     * If none is given, use 800x600.
     */
    if (argc == 3) {
        char* str = argv[2];
        char* pch;
        pch = strchr(str, 'x');
        if (pch == NULL) {
            printf("Resolution which was expected in the second argument should "
                    "be of the form of 1024x768");
            return false;
        }
        pch[0] = '\0';
        pch++;
        *resWidth = atoi(str);
        *resHeight = atoi(pch);
    }

    return true;
}

void scroller(Presentation* prs){
    // ====Scroller code start
    //TODO: make a function which will implement a calculation of imgIndexes
    // at the moment works only with three and more images in directory tmp/1
    IplImage *leftArrow = cvLoadImage("res/left.jpg", CV_LOAD_IMAGE_UNCHANGED);
    IplImage *rightArrow = cvLoadImage("res/right.jpg", CV_LOAD_IMAGE_UNCHANGED);
    Scroller scroller;
    //here are loaded all files which are in directory tmp/1
    scroller.loadFileNames();

    scroller.currentImg = 0;
    int movingArea;
    //default indexes - we'll show the first three pictures from array
    int imgIndex1=0, imgIndex2=1, imgIndex3=2;
    prs->drawScroller(prs, scroller.imgArray[imgIndex1], scroller.imgArray[imgIndex2],
                    scroller.imgArray[imgIndex3], leftArrow, rightArrow);
    //begin of cycle - we need to implement this part in for(;;) and to use inWhichAreaIsMoving
        //movingArea = Analysis::inWhichAreaIsMoving(leftArrow, rightArrow, prs);
        movingArea = 0; //0 or 4 - string for tests
        int position = scroller.imgArraySize - scroller.currentImg;
        if(movingArea == 0){ //left
            if(position == scroller.imgArraySize || position == 1) { //cycle
                if(position == 1) {
                    imgIndex1 = scroller.currentImg-1;
                    imgIndex2 = scroller.currentImg;
                    imgIndex3 = 0;
                } else {
                    imgIndex1 = scroller.imgArraySize - 1;
                    imgIndex2 = 0;
                    imgIndex3 = 1;
                }
            } else { //everything is okay
                imgIndex1 = scroller.currentImg-1;
                imgIndex2 = scroller.currentImg;
                imgIndex3 = scroller.currentImg+1;
            }
        } else if (movingArea == 4){ //right
            if(position > 3) { //everything is okay
                imgIndex1 = scroller.currentImg+1;
                imgIndex2 = scroller.currentImg+2;
                imgIndex3 = scroller.currentImg+3;
            } else { //cycle
                if(position == 3) {
                    imgIndex1 = scroller.currentImg+1;
                    imgIndex2 = scroller.currentImg+2;
                    imgIndex3 = 0;
                } else if (position == 2) {
                    imgIndex1 = scroller.currentImg+1;
                    imgIndex2 = 0;
                    imgIndex3 = 1;
                } else {
                    imgIndex1 = 0;
                    imgIndex2 = 1;
                    imgIndex3 = 2;
                }
            }
        }
        if (movingArea == 0 || movingArea == 4) {
            scroller.currentImg = imgIndex1;
            prs->drawScroller(prs, scroller.imgArray[imgIndex1], scroller.imgArray[imgIndex2],
                    scroller.imgArray[imgIndex3], leftArrow, rightArrow);
        }
        //end of cycle
   // ====Scroller code end
}

int main(int argc, char *argv[]) {
    Capture* cpt = 0;
    int startTime;
    int resWidth = 800;
    int resHeight = 600;
    
    if(!argHandler(argc, argv, &cpt, &resWidth, &resHeight)) return -1;

    //Code below is commented intentionally for testing purposes
    //And also in case of your curiosity you can test it

    //This part draws a rectangle on preuploaded testFrame
    //Upper left corner of rectangle is defined by template1 location in testFrame
    //Lower right corner of rectangle is defined by template2 location in testFrame
    /* IplImage *img1, *tmp1, *tmp2;
     img1 = cvLoadImage("/home/psyholog/iwb/frame.jpg", 0);
     tmp1 = cvLoadImage("/home/psyholog/NetBeansProjects/leftDownColorTmp.jpg", 0);
         tmp2 = cvLoadImage("/home/psyholog/NetBeansProjects/rightUpColorTmp.jpg", 0);
     cvRectangle(img1,
             Analysis::getLocation(img1,tmp1,true),
             Analysis::getLocation(img1,tmp2,false),
             cvScalar(0, 0, 255, 0), 1, 0, 0);
     cvNamedWindow("reference", CV_WINDOW_AUTOSIZE);
     cvShowImage("reference", img1);

     cvWaitKey(0);
     cvDestroyWindow("reference");
     cvReleaseImage(&img1);
     cvReleaseImage(&tmp1);
     cvReleaseImage(&tmp2);*/
    cpt->showDiff();

    Presentation* prs = new Presentation(resWidth, resHeight);
    Analysis::doCalibrate(cpt,prs);
    //Creating and loading Templates for making CI
    FILE *file = fopen("cj.txt", "r");
    IplImage *tmp1, *tmp2;
    tmp1 = cvLoadImage("res/Cleft.jpg", 1);
    tmp2 = cvLoadImage("res/Cright.jpg", 1);
    //making frame for test
    bool isSaved = false;
    const char* winFrame = "winFrame";
    cvNamedWindow(winFrame, CV_WINDOW_AUTOSIZE);
    int f;
    //bool currentKfSaved = false;
    //Controller
    IplImage* currentFrame = NULL;
    IplImage* cornerFrame = NULL;
    IplImage* diff = NULL;
    IplImage* Ci = NULL;
    IplImage* previousFrame = NULL;
    CvPoint p1, p2;

//    scroller(&prs);

    //=========================================================
    // ((( Control loop start
    for (;;) {
        
        //--------------------------------------------------------------------------------------------------------------
        // <<< Frame capture and release block
        
        //Capture current frame
        currentFrame = cvQueryFrame(cpt->getCapture());
        if (cpt->getPreviousFrame() == NULL) {
            // Occurs only on first iteration, avoids dereferencing of null-pointer
            cpt->setPreviousFrame(cvCloneImage(currentFrame));
            continue;
        }
        // Get previous frame
        previousFrame = cpt->getPreviousFrame();
        // Calculate difference between current and previous frame
        diff = Analysis::getDiff(previousFrame, currentFrame);
        // Release previous frame
        cvReleaseImage(&previousFrame);
        // Save current as previous frame for next iteration
        cpt->setPreviousFrame(cvCloneImage(currentFrame));

        // >>> Frame capture and release block
        //--------------------------------------------------------------------------------------------------------------

        
        // @todo Not sure where to place it:
        //int area = Analysis::inWhichAreaIsMoving(currentFrame, pr, point, width, height);

        //Just attempt to make timer
        if (!Analysis::isMoving(diff) && !isSaved) {
            if (startTime == -1) {
                startTime = clock() / CLOCKS_PER_SEC;
            }
            int timeDifference;
            timeDifference = clock() / CLOCKS_PER_SEC - startTime;
            if (timeDifference >= 2) {
                if (cornerFrame == NULL) {
                    cornerFrame = cvCloneImage(currentFrame);
                    startTime = -1;
                } else {
                    p1 = Analysis::getLocation(Analysis::getDiff(cornerFrame, currentFrame), tmp1, true);
                    p2 = Analysis::getLocation(Analysis::getDiff(cornerFrame, currentFrame), tmp2, false);
                    cpt->saveFrame("diff.jpg", Analysis::getDiff(cornerFrame, currentFrame));
                    cvRectangle(currentFrame, p1, p2, cvScalar(0, 0, 255, 0), 1, 0, 0);

                    //Counting coordinates for making CI
                    int width = p2.x - p1.x,
                            height = p2.y - p1.y;
                    try {

                        cvSetImageROI(currentFrame, cvRect(p1.x, p1.y, width, height));
                        Ci = cvCreateImage(cvGetSize(currentFrame), currentFrame->depth, currentFrame->nChannels);

                        cvCopyImage(currentFrame, Ci);
                        cvResetImageROI(currentFrame);
                        //Saving Images
                        cpt->saveFrame("CapturedImage.jpg", Ci);
                        cpt->saveFrame("KeyFrame.jpg", currentFrame);
                        //  currentKfSaved = true;

                        isSaved = true;
                        cvReleaseImage(&Ci);
                        //cvReleaseImage(&cornerFrame);
                    } catch (cv::Exception& e) {

                    }
                }
            }
        } else {
            startTime = -1;
            //  currentKfSaved = false;
            if (isSaved)
                prs->putImage(p1, p2, cvLoadImage("CapturedImage.jpg", 1));
            prs->applyBuffer();
        }
        cvShowImage(winFrame, currentFrame);
        cvWaitKey(5);
        cvReleaseImage(&diff);

        if (file != 0) {
            fscanf(file, "%d", &f);
            if (f == -15) {
                printf("stopped");
                fclose(file);
                return 0;
            }
        }
    }
    // ))) Control loop end
    //=========================================================

    return 0;
}