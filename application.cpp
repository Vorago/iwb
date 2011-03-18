#include "include/application.hpp"
#include <opencv/cv.h>
#include <cstdio>
#include <stdio.h>
#include "include/capture.hpp"
#include "include/analysis.hpp"
#include "include/presentation.hpp"
#include "include/scroller.hpp"
#include "include/handler.hpp"
#include "include/camera.hpp"

namespace iwb {

    int Application::initialize(int argc, char* argv[]) {
        hndl = new Handler();
        cpt = NULL;

        int resWidth = 1024;
        int resHeight = 768;

        if (!hndl->handleArguments(argc, argv, &cpt, &resWidth, &resHeight)) return -1;

        prs = new Presentation(resWidth, resHeight);
        Camera::getInstance()->calibrate(cpt, prs);
        Scroller* scroller = new Scroller();

        return 0;
    }

    int Application::run() {
    //    cpt->showDiff();

    //    Analysis::doCalibrate(cpt, prs);
        //Creating and loading Templates for making CI

        int startTime;
        FILE *file = fopen("cj.txt", "r");
        IplImage *tmp1, *tmp2;
        tmp1 = cvLoadImage("res/Cleft.jpg", 0);
        tmp2 = cvLoadImage("res/Cright.jpg", 0);
        //making frame for test
        bool isSaved = false;
        const char* winFrame = "winFrame";
        cvNamedWindow(winFrame, CV_WINDOW_AUTOSIZE);
        cvNamedWindow("TestFrame", CV_WINDOW_AUTOSIZE);
        int f;
        //bool currentKfSaved = false;
        //Controller
        IplImage* currentFrame = NULL;
        IplImage* cornerFrame = NULL;
        IplImage* diff = NULL;
        IplImage* Ci = NULL;
        IplImage* previousFrame = NULL;
        CvPoint p1, p2;

//    scroller(prs);


    //=========================================================
    // ((( Control loop start
//        cpt->showDiff();

        for (;;) {

            prs->drawConfirmation();

            /**/
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
    //            prs->drawConfirmation();
                prs->applyBuffer();
            }
         /**/
            cvShowImage(winFrame, currentFrame);
            cvShowImage("ourWinDiff", diff);

            cvWaitKey(40);
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
    /**/

    return 0;
    }
}
