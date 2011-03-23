/* 
 * File:   analysis.cpp
 * Author: vorago
 * 
 * Created on December 23, 2010, 1:10 PM
 */

#include "include/analysis.hpp"
#include "include/constants.hpp"
#include <opencv/highgui.h>
#include <stdio.h>

namespace iwb {

    IplImage* Analysis::getCornerDiff(IplImage* withoutCorners, IplImage* withCorners) {
        IplImage* diff;
        IplImage* image2;
        IplImage* image1;
        image1 = cvCreateImage(cvGetSize(withoutCorners), withoutCorners->depth, 1);
        image2 = cvCreateImage(cvGetSize(withCorners), withoutCorners->depth, 1);
        diff = cvCreateImage(cvGetSize(withCorners), withoutCorners->depth, 1);
        cvCvtColor(withoutCorners, image1, CV_BGR2GRAY);
        cvCvtColor(withCorners, image2, CV_BGR2GRAY);

        cvAbsDiff(image1, image2, diff);
        cvThreshold(diff, diff, constants::threshold, 255, CV_THRESH_BINARY);
//        cvSmooth(diff, image2, CV_AA, 1);
        
        return diff;
    }

    IplImage* Analysis::getMovementDiff() {
        return getDiff();
    }

    Analysis::Analysis(Capture* cpt) {
        this->cpt = cpt;

        initialize();
    }

    Analysis::~Analysis() {

    }

    void Analysis::initialize() {
        bg = cvCloneImage(cvQueryFrame(cpt->getCapture()));
//        cvWaitKey(3000);
//        bg = cvCloneImage(cvQueryFrame(cpt->getCapture()));
    }

    void Analysis::update() {
//        bool update;
//        while (update) {
//            IplImage* currentFrame = );
//
//            // Calculate difference between current and previous frame
//            diff = Analysis::getCornerDiff(previousFrame, currentFrame);
//            // Release previous frame
//             Save current as previous frame for next iteration
//            previousFrame = cvCloneImage(cvQueryFrame(cpt->getCapture()));
//
//            // >>> Frame capture and release block
//            //--------------------------------------------------------------------------------------------------------------
//
//
//            // @todo load function here
//            //int area = Analysis::inWhichAreaIsMoving(currentFrame, pr, point, width, height);
//
//            //Just attempt to make timer
//            if (!isMoving(diff) && !isSaved) {
//                if (startTime == -1) {
//                    startTime = clock() / CLOCKS_PER_SEC;
//                }
//                int timeDifference;
//                timeDifference = clock() / CLOCKS_PER_SEC - startTime;
//                if (timeDifference >= 2) {
//                    // Image stands still for some time
//                    if (cornerFrame == NULL) {
//                        // First time capturing still frame
//                        cornerFrame = cvCloneImage(currentFrame);
//                        puts("first");
//                        startTime = -1;
//                    } else {
//                        // Second time capturing still frame
//                        p1 = Analysis::getLocation(Analysis::getDiff(cornerFrame, currentFrame), tmp1, true);
//                        p2 = Analysis::getLocation(Analysis::getDiff(cornerFrame, currentFrame), tmp2, false);
//                        cpt->saveFrame("diff.jpg", Analysis::getDiff(cornerFrame, currentFrame));
//                        cvRectangle(currentFrame, p1, p2, cvScalar(0, 0, 255, 0), 1, 0, 0);
//
//                        //------------------------------------------------------------------------------
//                        // [[[ Save KF & CI
//                        //Counting coordinates for making CI
//                        int width = p2.x - p1.x,
//                                height = p2.y - p1.y;
//                        try {
//                            cvSetImageROI(currentFrame, cvRect(p1.x, p1.y, width, height));
//                            Ci = cvCreateImage(cvGetSize(currentFrame), currentFrame->depth, currentFrame->nChannels);
//
//                            cvCopyImage(currentFrame, Ci);
//                            cvResetImageROI(currentFrame);
//                            // Saving Images
//                            // @todo save function here
//                            cpt->saveFrame("CapturedImage.jpg", Ci);
//                            cpt->saveFrame("KeyFrame.jpg", currentFrame);
//                            //currentKfSaved = true;
//
//                            isSaved = true;
//                            cvReleaseImage(&Ci);
//                            //cvReleaseImage(&cornerFrame);
//                        } catch (cv::Exception& e) {
//
//                        }
//                        // ]]] Save KF & CI
//                        //------------------------------------------------------------------------------
//                    }
//                }
//            } else {
//                startTime = -1;
//                //  currentKfSaved = false;
//                // Display saved frame
//                if (isSaved)
//                    prs->putImage(p1, p2, cvLoadImage("CapturedImage.jpg", 1));
//                prs->applyBuffer();
//            }
//        }
    }

    IplImage* Analysis::getDiff() {
        CvMat cfmat;

        IplImage *dst_img = cvCreateImage(cvGetSize(bg), bg->depth, bg->nChannels),
                 *gs = cvCreateImage(cvGetSize(bg), IPL_DEPTH_8U, 1),
                 *blur = cvCreateImage(cvGetSize(bg), bg->depth, bg->nChannels),
                 *bw = cvCreateImage(cvGetSize(bg), IPL_DEPTH_8U, 1);

        IplImage* cf = cvQueryFrame(cpt->getCapture());
            cfmat = cv::cvarrToMat(cf);
            cvSub(bg, cf, dst_img, NULL);
            cvSmooth(dst_img, blur, CV_BLUR, 5);
            cvCvtColor(blur, gs,CV_RGB2GRAY);
            cvThreshold(gs,bw,128,255,CV_THRESH_TRUNC/*|CV_THRESH_OTSU*/);
            cvThreshold(bw,gs,12,255,CV_THRESH_BINARY/*|CV_THRESH_OTSU*/);
//            cvCvtColor(gs,dst_img,CV_GRAY2RGB);

            return gs;
    }

    CvPoint Analysis::getLocation(IplImage *source, IplImage *pattern, bool upperLeft) {
        IplImage* matchRes;
        double minVal, maxVal;
        CvPoint minLoc, maxLoc;
        matchRes = cvCreateImage(cvSize(
                source->width - pattern->width + 1,
                source->height - pattern->height + 1
                ), IPL_DEPTH_32F, 1);

        cvMatchTemplate(source, pattern, matchRes, CV_TM_SQDIFF);
        cvMinMaxLoc(matchRes, &minVal, &maxVal, &minLoc, &maxLoc, 0);

        cvReleaseImage(&matchRes);
        if (!upperLeft) {
            minLoc.x += pattern->width;
            minLoc.y += pattern->height;
//            maxLoc.x += pattern->width;
//            maxLoc.y += pattern->height;
//            return maxLoc;
        }
        return minLoc;
    }

    int Analysis::inWhichAreaIsMoving(IplImage* curr, IplImage* prev, Presentation* prs) {
//        int area;
//
//        CvPoint point[] = {prs->leftUL, prs->img1UL, prs->img2UL, prs->img3UL, prs->rightUL};
//        int width[] = {prs->leftBR.x - prs->leftUL.x, prs->img1BR.x - prs->img1UL.x,
//            prs->img2BR.x - prs->img2UL.x, prs->img3BR.x - prs->img3UL.x, prs->rightBR.x - prs->rightUL.x};
//        int height[] = {prs->leftBR.y - prs->leftUL.y, prs->img1BR.y - prs->img1UL.y,
//            prs->img2BR.y - prs->img2UL.y, prs->img3BR.y - prs->img3UL.y, prs->rightBR.y - prs->rightUL.y};
//
//        for (int i = 0; i < 5; i++) {
//            cvSetImageROI(curr, cvRect(point[i].x, point[i].y, width[i], height[i]));
//            cvSetImageROI(prev, cvRect(point[i].x, point[i].y, width[i], height[i]));
//            IplImage* diff = Analysis::getDiff(curr, prev);
//            cvResetImageROI(curr);
//            cvResetImageROI(prev);
//            if (isMoving(diff)) {
//                cvReleaseImage(&diff);
//                return area = i;
//            }
//            cvReleaseImage(&diff);
//        }

        return -1;
    }

    bool Analysis::isMoving(const IplImage* diff) {
        bool isWhite = false;
        int resolution = diff->height * diff->width;
        const char* data = diff->imageData;

        if (diff->depth == IPL_DEPTH_8U) {
            for (int i = 0; i < resolution * diff->nChannels; i++) {
                if ((uchar) data[i] == 255) {
                    isWhite = true;
                    break;
                }
            }
        }
        return isWhite;
    }

    void Analysis::doCalibrate(Capture* cpt, Presentation* prs) {
//        
//        //testScreen shoul be a big rectangle of blue color with black square in upper-left corner and a red one in bottom-right
//        IplImage* blackScreen = cvLoadImage("res/bg.jpg", 1);
//        //blackSquare should match size of square in upper left of testScreen
//
////        IplImage* blackSquare = cvLoadImage("res/CleftCalib.jpg",1);
//        IplImage* blackSquare = cvLoadImage("res/Cleft.jpg",1);
//
//        //redSquare should match size of square in bottom right of testScreen
////        IplImage* redSquare = cvLoadImage("res/CrightCalib.jpg", 1);
//        IplImage* redSquare = cvLoadImage("res/Cright.jpg", 1);
//
//        //redSquare should match size of square in bottom right of testScreen
//        IplImage* redSquare2 = cvLoadImage("res/Cright.jpg",0);
//
//          IplImage* blackSquare2 = cvLoadImage("res/Cleft.jpg", 0);
//
//        prs->putImage(cvPoint(0, 0), cvPoint(prs->getScreenWidth(), prs->getScreenHeight()), blackScreen);
//        prs->applyBuffer();
//
//        cvWaitKey(500);
//        IplImage* frame =NULL;
//        IplImage* frame2;
//
//        for(int i=0;i<200;i++){
//            if(i==0){
//                cvQueryFrame(cpt->getCapture());
//                frame = cvQueryFrame(cpt->getCapture());
//                frame2=cvCloneImage(frame);
//                cpt->saveFrame("f1Copy.jpg", frame2);
//            }
//            cvQueryFrame(cpt->getCapture());
//            if(i==100){
//                prs->putImage(cvPoint(10, 10), cvPoint(64, 48), blackSquare);
//                prs->putImage(cvPoint(900, 600), cvPoint(950, 650), redSquare);
//                prs->applyBuffer();
//                cvWaitKey(100);
//            }
//            if(i==199){
//                frame = cvQueryFrame(cpt->getCapture());
//            }
//        }
//
//
//        frame2 = cvLoadImage("f1Copy.jpg", 1);
//        cpt->saveFrame("f1.jpg", frame2);
//        cpt->saveFrame("f2.jpg", frame);
//        IplImage *diff = Analysis::getDiff(frame2, frame);
//
//        cpt->saveFrame("diff.jpg", diff);
//        CvPoint ul = Analysis::getLocation(diff, blackSquare2, true);
//        CvPoint br = Analysis::getLocation(diff, redSquare2, false);
//        cvReleaseImage(&diff);
//        cvReleaseImage(&blackScreen);
//        cvReleaseImage(&blackSquare);
//        cvReleaseImage(&redSquare);
//
//
//
//        printf("%d\n", ul.x);
//        printf("%d\n", ul.y);
//        printf("--------\n");
//        printf("%d\n", br.x);
//        printf("%d\n", br.y);
//        printf("--------\n");
//
//
//        //set camera resolution depending on captured frame
//        cpt->screenHeight = frame->height;
//        cpt->screenWidth = frame->width;
////        printf("%d\n", cpt->screenWidth);
//   //     printf("%d\n", cpt->screenHeight);
//      //  printf("--------\n");
//       // cpt->saveFrame("calibrate.jpg", frame);
//        prs->leftOffset = ul.x / cpt->screenWidth;
//        prs->rightOffset = (prs->getScreenWidth() - br.x) / cpt->screenWidth;
//        prs->topOffset = ul.y / cpt->screenHeight;
//        prs->bottomOffset = (prs->getScreenHeight() - br.y) / cpt->screenHeight;
//
//        /* Each image occupies 17% of scroller height and 67% of scroller lenght
//         * Each arrow occupies 7% of scroller height and 50% of scroller lenght
//         * Space between images and images and arrows - 7%
//         */
//        //calculations of relative coordinates
//
//        //@todo export those magic numbers to constants and recalc them
//        CvPoint upperLeft = cvPoint(0, 0);
//        CvPoint lowerRight = cvPoint(800, 200);
//
//        int scrollerWidth = lowerRight.x - upperLeft.x;
//        int scrollerHeight = lowerRight.y - upperLeft.y;
//
//        prs->scrollerUL[0] = cvPoint(
//                upperLeft.x + round(0.05 * scrollerWidth),
//                upperLeft.y + round(0.25 * scrollerHeight)
//                );
//        prs->scrollerBR[0] = cvPoint(
//                upperLeft.x + round(0.12 * scrollerWidth),
//                upperLeft.y + round(0.75 * scrollerHeight)
//                );
//
//        prs->scrollerUL[2] = cvPoint(
//                upperLeft.x + round(0.41 * scrollerWidth),
//                upperLeft.y + round(0.17 * scrollerHeight)
//                );
//        prs->scrollerBR[2] = cvPoint(
//                upperLeft.x + round(0.58 * scrollerWidth),
//                upperLeft.y + round(0.84 * scrollerHeight)
//                );
//
//        prs->scrollerUL[3] = cvPoint(
//                upperLeft.x + round(0.65 * scrollerWidth),
//                upperLeft.y + round(0.17 * scrollerHeight)
//                );
//        prs->scrollerBR[3] = cvPoint(
//                upperLeft.x + round(0.82 * scrollerWidth),
//                upperLeft.y + round(0.84 * scrollerHeight)
//                );
//
//        prs->scrollerUL[4] = cvPoint(
//                upperLeft.x + round(0.88 * scrollerWidth),
//                upperLeft.y + round(0.25 * scrollerHeight)
//                );
//        prs->scrollerBR[4] = cvPoint(
//                upperLeft.x + round(0.95 * scrollerWidth),
//                upperLeft.y + round(0.75 * scrollerHeight)
//                );
//
//        //@todo export those magic numbers to constants and recalc them
//        upperLeft = cvPoint(850, 0);
//        lowerRight = cvPoint(1000, 200);
//
//        int confirmWidth = lowerRight.x - upperLeft.x;
//        int confirmHeight = lowerRight.y - upperLeft.y;
//
//        printf("confirmWidth: %d, confirmHeight: %d", confirmWidth, confirmHeight);
//
//        prs->confirmationUL[0] = cvPoint(
//                upperLeft.x + round(0.00 * confirmWidth),
//                upperLeft.y + round(0.25 * confirmHeight)
//                );
//        prs->confirmationBR[0] = cvPoint(
//                upperLeft.x + round(0.45 * confirmWidth),
//                upperLeft.y + round(0.75 * confirmHeight)
//                );
//
//        prs->confirmationUL[1] = cvPoint(
//                upperLeft.x + round(0.50 * scrollerWidth),
//                upperLeft.y + round(0.25 * scrollerHeight)
//                );
//        prs->confirmationBR[1] = cvPoint(
//                upperLeft.x + round(0.95 * scrollerWidth),
//                upperLeft.y + round(0.75 * scrollerHeight)
//                );
//        //formulas here for capture..
//        //coeff = camW*(1-left-right)/projW
//
//        cpt->scrollerUL[0] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->scrollerUL[0].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->scrollerUL[0].y
//                );
//        cpt->scrollerBR[0] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->scrollerUL[0].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->scrollerUL[0].y
//                );
//
//        cpt->scrollerUL[1] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->scrollerUL[1].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->scrollerUL[1].y
//                );
//        cpt->scrollerBR[1] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->scrollerBR[1].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->scrollerBR[1].y
//                );
//
//        cpt->scrollerUL[2] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->scrollerUL[2].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->scrollerUL[2].y
//                );
//        cpt->scrollerBR[2] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->scrollerBR[2].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->scrollerBR[2].y
//                );
//
//        cpt->scrollerUL[3] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->scrollerUL[3].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->scrollerUL[3].y
//                );
//        cpt->scrollerBR[3] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->scrollerBR[3].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->scrollerBR[3].y
//                );
//
//        cpt->scrollerUL[4] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->scrollerUL[4].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->scrollerUL[4].y
//                );
//        cpt->scrollerBR[4] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->scrollerBR[4].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->scrollerBR[4].y
//                );
//        //cvReleaseImage(&frame);
//
//        cpt->confirmationUL[0] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->confirmationUL[0].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->confirmationUL[0].y
//                );
//        cpt->confirmationBR[0] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->confirmationUL[0].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->confirmationUL[0].y
//                );
//
//        cpt->confirmationUL[1] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->confirmationUL[1].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->confirmationUL[1].y
//                );
//        cpt->confirmationBR[1] = cvPoint(
//                prs->leftOffset * cpt->screenWidth + ((cpt->screenWidth * (1 - prs->leftOffset - prs->rightOffset)) / prs->screenWidth) * prs->confirmationBR[1].x,
//                prs->topOffset * cpt->screenHeight + ((cpt->screenHeight * (1 - prs->topOffset - prs->bottomOffset)) / prs->screenHeight) * prs->confirmationBR[1].y
//                );
    }
}