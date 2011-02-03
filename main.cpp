//#include <cstdio>
#include "include/capture.hpp"
#include "include/analysis.hpp"
#include "include/presentation.hpp"



using namespace iwb;

int main(int argc, char *argv[]) {

    int cam;
    Capture* cpt = 0;
    // Argument handling block
    if (argc >= 2) {
        char *endptr;
        cam = strtol(argv[1], &endptr, 10);
        if (*endptr == '\0') {
            cpt = new Capture(cam);
        } else {
            cpt = new Capture(argv[1]);
        }
    } else {
        cam = 0;
        cpt = new Capture(cam);
    }

    /*
     * Process resolution in second parameter.
     * If none is given, use 800x600.
     */
    int resWidth = 800;
    int resHeight = 600;
    if (argc == 3) {
        char* str = argv[2];
        char* pch;
        pch = strchr(str, 'x');
        if (pch == NULL) {
            printf("Resolution which was expected in the second argument should "
                    "be of the form of 1024x768");
            return -1;
        }
        pch[0] = '\0';
        pch++;
        resWidth = atoi(str);
        resHeight = atoi(pch);
    }

    /*  Presentation* prs = new Presentation(resWidth, resHeight);
      IplImage* img = cvLoadImage("/home/psyholog/kruze.jpg", CV_LOAD_IMAGE_UNCHANGED);
    
      prs->putImage(cvPoint(100, 100), cvPoint(500, 700), img);
      prs->putImage(cvPoint(100, 100), cvPoint(500, 700), img);
      prs->applyBuffer();
      cvWaitKey(0);
      prs->clearArea(cvPoint(110, 110), cvPoint(200, 400));*/

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
    //cpt->showDiff();


    //Creating and loading Templates for making CI
    IplImage *tmp1, *tmp2;
    tmp1 = cvLoadImage("/home/psyholog/NetBeansProjects/leftUpColorTmp2.jpg", 1);
    tmp2 = cvLoadImage("/home/psyholog/NetBeansProjects/rightDownColorTmp2.jpg", 1);
    //making frame for test
    const char* winFrame = "winFrame";
    cvNamedWindow(winFrame, CV_WINDOW_AUTOSIZE);

    bool currentKfSaved = false;
    //Controller
    for (;;) {
        IplImage* currentFrame = cvQueryFrame(cpt->getCapture());
        if (cpt->getPreviousFrame() == NULL) {
            cpt->setPreviousFrame(cvCloneImage(currentFrame));
            continue;
        }
        IplImage* diff = Analysis::getDiff(cpt->getPreviousFrame(), currentFrame);
        cpt->setPreviousFrame(cvCloneImage(currentFrame));
        //Just attempt to make timer
        int startTime;
        if (!Analysis::isMoving(diff)) {
            if (startTime == -1) {
                startTime = clock() / CLOCKS_PER_SEC;
            }
            int timeDifference;
            timeDifference = clock() / CLOCKS_PER_SEC - startTime;
            if (timeDifference >= 2 && !currentKfSaved) {
                CvPoint p1 = Analysis::getLocation(currentFrame, tmp1, true),
                        p2 = Analysis::getLocation(currentFrame, tmp2, false);
                cvRectangle(currentFrame, p1, p2, cvScalar(0, 0, 255, 0), 1, 0, 0);
                //Counting coordinates for making CI
                int width = p2.x - p1.x,
                        height = p2.y - p1.y;
                try {
                    cvSetImageROI(currentFrame, cvRect(p1.x, p1.y, width, height));
                    IplImage* Ci = cvCreateImage(cvGetSize(currentFrame), currentFrame->depth, currentFrame->nChannels);
                    cvCopyImage(currentFrame, Ci);
                    cvResetImageROI(currentFrame);
                    //Saving Images
                    cpt->saveFrame("/home/psyholog/NetBeansProjects/CapturedImage.jpg", Ci);
                    cpt->saveFrame("/home/psyholog/NetBeansProjects/KeyFrame.jpg", currentFrame);
                    currentKfSaved = true;
                    cvReleaseImage(&Ci);
                } catch (cv::Exception& e) {
                    //If something goes wrong
                }
            }
        } else {
            startTime = -1;
            currentKfSaved = false;
        }
        cvShowImage(winFrame, currentFrame);
        cvWaitKey(5);
        cvReleaseImage(&diff);
    }

    return 0;
}
