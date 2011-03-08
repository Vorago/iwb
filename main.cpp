//#include <cstdio>
#include "include/capture.hpp"
#include "include/analysis.hpp"
#include "include/presentation.hpp"
#include "include/scroller.hpp"
#include "include/handler.hpp"

using namespace iwb;

void  buttons(Presentation* prs){
    IplImage *yesButton = cvLoadImage("res/yesButton.jpg", CV_LOAD_IMAGE_UNCHANGED);
    IplImage *noButton = cvLoadImage("res/noButton.jpg", CV_LOAD_IMAGE_UNCHANGED);

    prs->putImage(cvPoint(prs->buttonsUL[0].x, prs->buttonsUL[0].y), cvPoint(prs->buttonsBR[0].x, prs->buttonsBR[0].y), yesButton);
    prs->putImage(cvPoint(prs->buttonsUL[1].x, prs->buttonsUL[1].y), cvPoint(prs->buttonsBR[1].x, prs->buttonsBR[1].y), yesButton);
}

void scroller(Presentation* prs) {
    // ====Scroller code start
    //TODO: make a function which will implement a calculation of imgIndexes
    // at the moment works only with three and more images in directory tmp/1
    IplImage *leftArrow = cvLoadImage("res/left.jpg", CV_LOAD_IMAGE_UNCHANGED);
    IplImage *rightArrow = cvLoadImage("res/right.jpg", CV_LOAD_IMAGE_UNCHANGED);
    Scroller scroller;
    //here are loaded all files which are in directory tmp/1
    scroller.loadFileNames();
    scroller.currentImg = 1;
    int movingArea;
    //default indexes - we'll show the first three pictures from array
    int imgIndex1 = 0, imgIndex2 = 1, imgIndex3 = 2;
    prs->drawScroller(prs, scroller.imgArray[imgIndex1], scroller.imgArray[imgIndex2],
            scroller.imgArray[imgIndex3], leftArrow, rightArrow);
    //begin of cycle - we need to implement this part in for(;;) and to use inWhichAreaIsMoving
    //movingArea = Analysis::inWhichAreaIsMoving(leftArrow, rightArrow, prs);
    movingArea = 4; //0 or 4 - string for tests
    int position = scroller.imgArraySize - scroller.currentImg;
    if (movingArea == 0) { //left
        if (position == scroller.imgArraySize || position == 1) { //cycle
            if (position == 1) {
                imgIndex1 = scroller.currentImg - 1;
                imgIndex2 = scroller.currentImg;
                imgIndex3 = 0;
            } else {
                imgIndex1 = scroller.imgArraySize - 1;
                imgIndex2 = 0;
                imgIndex3 = 1;
            }
        } else { //everything is okay
            imgIndex1 = scroller.currentImg - 1;
            imgIndex2 = scroller.currentImg;
            imgIndex3 = scroller.currentImg + 1;
        }
    } else if (movingArea == 4) { //right
        if (position > 3) { //everything is okay
            imgIndex1 = scroller.currentImg + 1;
            imgIndex2 = scroller.currentImg + 2;
            imgIndex3 = scroller.currentImg + 3;
        } else { //cycle
            if (position == 3) {
                imgIndex1 = scroller.currentImg + 1;
                imgIndex2 = scroller.currentImg + 2;
                imgIndex3 = 0;
            } else if (position == 2) {
                imgIndex1 = scroller.currentImg + 1;
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
    Handler* hndl =  new Handler();
    Capture* cpt = 0;
    int startTime;
    int resWidth = 1024;
    int resHeight = 768;
    
    if (!hndl->handleArguments(argc, argv, &cpt, &resWidth, &resHeight)) return -1;

    Presentation* prs = new Presentation(resWidth, resHeight);cpt->showDiff();

    Analysis::doCalibrate(cpt, prs);
    //Creating and loading Templates for making CI
    FILE *file = fopen("cj.txt", "r");
    IplImage *tmp1, *tmp2;
    tmp1 = cvLoadImage("res/Cleft.jpg", 0);
    tmp2 = cvLoadImage("res/Cright.jpg", 0);
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

    scroller(prs);
    buttons(prs);

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


        // @todo load function here
        //int area = Analysis::inWhichAreaIsMoving(currentFrame, pr, point, width, height);

        //Just attempt to make timer
        if (!Analysis::isMoving(diff) && !isSaved) {
            if (startTime == -1) {
                startTime = clock() / CLOCKS_PER_SEC;
            }
            int timeDifference;
            timeDifference = clock() / CLOCKS_PER_SEC - startTime;
            if (timeDifference >= 2) {
                // Image stands still for some time
                if (cornerFrame == NULL) {
                    // First time capturing still frame
                    cornerFrame = cvCloneImage(currentFrame);
                    puts("first");
                    startTime = -1;
                } else {
                    // Second time capturing still frame
                    p1 = Analysis::getLocation(Analysis::getDiff(cornerFrame, currentFrame), tmp1, true);
                    p2 = Analysis::getLocation(Analysis::getDiff(cornerFrame, currentFrame), tmp2, false);
                    cpt->saveFrame("diff.jpg", Analysis::getDiff(cornerFrame, currentFrame));
                    cvRectangle(currentFrame, p1, p2, cvScalar(0, 0, 255, 0), 1, 0, 0);

                    //------------------------------------------------------------------------------
                    // [[[ Save KF & CI
                    //Counting coordinates for making CI
                    int width = p2.x - p1.x,
                            height = p2.y - p1.y;
                    try {
                        cvSetImageROI(currentFrame, cvRect(p1.x, p1.y, width, height));
                        Ci = cvCreateImage(cvGetSize(currentFrame), currentFrame->depth, currentFrame->nChannels);

                        cvCopyImage(currentFrame, Ci);
                        cvResetImageROI(currentFrame);
                        // Saving Images
                        // @todo save function here
                        cpt->saveFrame("CapturedImage.jpg", Ci);
                        cpt->saveFrame("KeyFrame.jpg", currentFrame);
                        //currentKfSaved = true;

                        isSaved = true;
                        cvReleaseImage(&Ci);
                        //cvReleaseImage(&cornerFrame);
                    } catch (cv::Exception& e) {

                    }
                    // ]]] Save KF & CI
                    //------------------------------------------------------------------------------
                }
            }
        } else {
            startTime = -1;
            //  currentKfSaved = false;
            // Display saved frame
            if (isSaved)
                prs->putImage(p1, p2, cvLoadImage("CapturedImage.jpg", 1));
            prs->applyBuffer();
        }
        cvShowImage(winFrame, currentFrame);
        cvWaitKey(5);
        cvReleaseImage(&diff);

        //--------------------------------------------------
        // Check if "stop" button was pressed
        if (file != 0) {
            fscanf(file, "%d", &f);
            if (f == -15) {
                printf("stopped");
                fclose(file);
                return 0;
            }
        }
        //-------------------------------------------------
    }
    // ))) Control loop end
    //=========================================================

    return 0;
}