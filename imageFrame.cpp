#include "include/imageFrame.hpp"
#include "include/confirmation.hpp"
#include "include/camera.hpp"
#include <opencv/cv.h>

namespace iwb {
    ImageFrame::ImageFrame(Capture* cpt, Presentation* prs, Analysis* analysis) : Drawable("res/yes.jpg", prs, cvPoint(0, 0), cvPoint(1024, 768)) {
        this->cpt = cpt;
        this->analysis = analysis;

        ulImage = cvLoadImage("res/CleftSmaller.jpg", 0);
        brImage = cvLoadImage("res/CrightSmaller.jpg", 0);

        startTime = -1;
        currentProcess = DRAWING;
        captureState = IDLE;

        // initializing camera coordinates
        // FIXME: replace with actual camera coordinates
        cameraUL = cvPoint(0, 0);
        cameraBR = cvPoint(1024, 768);
//        refreshCornerCoords(time);

//        IplImage* currentFrame = cpt->getCapture();
//                    p1 = Analysis::getLocation(Analysis::getDiff(cornerFrame, currentFrame), tmp1, true);
//                    p2 = Analysis::getLocation(Analysis::getDiff(cornerFrame, currentFrame), tmp2, false);
    }

    void ImageFrame::refreshCornerCoords(IplImage* currentFrame) {
//        bool calculate = true;
//        while (firstTime || calculate) {
//            CvPoint ul = Analysis::getLocation(analysis->getCornerDiff(), ulImage, true);
//            CvPoint br = Analysis::getLocation(analysis->getCornerDiff(), brImage, false);
//
//            if (ul.x < br.x && ul.y < br.y) {
//                projectorUL = ul;
//                projectorBR = br;
//
//                break;
//            }
//
//            calculate = false;
//        }
                        // Second time capturing still frame
        IplImage* diff = analysis->getCornerDiff(capturedFrame, currentFrame);
                        cameraUL = analysis->getLocation(diff, ulImage, true);
                        cameraBR = analysis->getLocation(diff, brImage, false);
//                        cvRectangle(diff, projectorUL, projectorBR, cvScalar(0, 0, 255, 0), 1, 0, 0);
        cpt->saveFrame("cornerDiff.jpg", diff);

                        printf("if ul.x: %d, ul.y: %d\n", cameraUL.x, cameraUL.y);
                        printf("if br.x: %d, br.y: %d\n", cameraBR.x, cameraBR.y);
//                        cpt->saveFrame("diff.jpg", Analysis::getDiff(capturedFrame, currentFrame));
                        // ]]] Save KF & CI
                        //------------------------------------------------------------------------------
    }

    void ImageFrame::captureFrame(IplImage* currentFrame) {
//                        cvRectangle(currentFrame, cameraUL, cameraBR, cvScalar(0, 0, 255, 0), 1, 0, 0);

                        //------------------------------------------------------------------------------
                        // [[[ Save KF & CI
                        //Counting coordinates for making CI
                        int width = cameraBR.x - cameraUL.x,
                                height = cameraBR.y - cameraUL.y;
                        try {
                            cvSetImageROI(currentFrame, cvRect(cameraUL.x, cameraUL.y, width, height));
                            IplImage* Ci = cvCreateImage(cvGetSize(currentFrame), currentFrame->depth, currentFrame->nChannels);

                            if (currentFrame != NULL)
                                cvReleaseImage(&currentFrame);
                            cvCopyImage(currentFrame, Ci);
                            capturedFrame = cvCloneImage(Ci);

                            cvResetImageROI(currentFrame);
//                            // Saving Images
//                            // @todo save function here
//                            cpt->saveFrame("KeyFrame.jpg", currentFrame);
//                            //currentKfSaved = true;
//
//                            isSaved = true;
                            cvReleaseImage(&Ci);
                            //cvReleaseImage(&capturedFrame);
                        } catch (cv::Exception& e) {

                        }
    }

    void ImageFrame::setImagePath(const char* imagePath) {
        if (captureState == IDLE && currentProcess == DRAWING) {
            cvReleaseImage(&image);
            image = cvLoadImage(imagePath, CV_LOAD_IMAGE_UNCHANGED);
            currentProcess = CHANGING_IMAGE;
            captureState = GETTING_CAPTURE;
        }
    }

    void ImageFrame::saveFrame() {
        if (captureState == IDLE && currentProcess == DRAWING) {
            currentProcess = SAVING_IMAGE;
            captureState = GETTING_CAPTURE;
        }
    }

    void ImageFrame::checkForCapture() {
        refreshCornerCoords(NULL);
    }

    void ImageFrame::checkForMovement() {
        if (captureState == IDLE) {
            return;
        }

        IplImage* currentFrame = cvQueryFrame(cpt->getCapture());
        if (cpt->getPreviousFrame() == NULL) {
            cpt->setPreviousFrame(cvCloneImage(currentFrame));
            return;
        }
        
        IplImage* previousFrame = cpt->getPreviousFrame();

        IplImage* diff = analysis->getCornerDiff(previousFrame, currentFrame);
        
        cpt->setPreviousFrame(cvCloneImage(currentFrame));

//        int startTime = -1;

        if (!analysis->isMoving(diff) && (captureState == GETTING_CAPTURE || captureState == WAITING_FOR_CORNERS)) {
            printf("DEBUG: not moving\n");
            if (startTime == -1) {
                printf("DEBUG: clock started\n");
                startTime = clock() / CLOCKS_PER_SEC;
            }
            int timeDifference;
            timeDifference = clock() / CLOCKS_PER_SEC - startTime;
            if (timeDifference >= 2) {
                printf("DEBUG: clock triggered\n");
                switch (captureState) {
                    case GETTING_CAPTURE:
                        capturedFrame = cvCloneImage(currentFrame);
                        puts("first");
                        startTime = -1;
                        captureState = WAITING_FOR_CORNERS;
                        break;
                    case WAITING_FOR_CORNERS:
                        refreshCornerCoords(currentFrame);
                        puts("second");
                        // TODO: pass the saveFrame function as a callback to the confirmation dialog for touched yes event
                        captureState = CAPTURED;
                        break;
                }
            }
        } else {
            startTime = -1;
        }

        cvReleaseImage(&diff);
    }

    void ImageFrame::update() {
//        checkForMovement();
        printf("DEBUG: imageFrame current process: %d\n", currentProcess);
        printf("DEBUG: imageFrame capture state: %d\n", captureState);

        switch (currentProcess) {
            case DRAWING:
                break;
            case CHANGING_IMAGE:
                checkForMovement();
                if (captureState == CAPTURED) {
                    captureState = IDLE;
                    currentProcess = DRAWING;
                }
                break;
            case SAVING_IMAGE:
                checkForMovement();
                if (captureState == CAPTURED) {
                    captureFrame(cpt->getPreviousFrame());
                    cpt->saveFrame("CapturedImage.jpg", capturedFrame);
                    
                    captureState = IDLE;
                    currentProcess = DRAWING;
                }
                break;
        }

        if (captureState == CAPTURED) {
            startTime = -1;
            cvReleaseImage(&capturedFrame);
            capturedFrame = NULL;
        }
    }

    void ImageFrame::draw(Presentation* prs) {
        if (currentProcess == DRAWING || currentProcess == SAVING_IMAGE) {
//            prs->putImage(projectorUL, projectorBR, image);
            prs->putImage(cameraUL, cameraBR, NULL, NULL, image);
            prs->applyBuffer();
        }
    }
}
