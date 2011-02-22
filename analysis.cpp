/* 
 * File:   analysis.cpp
 * Author: vorago
 * 
 * Created on December 23, 2010, 1:10 PM
 */

#include "include/analysis.hpp"
#include "include/constants.hpp"

namespace iwb {

    IplImage* Analysis::getDiff(IplImage *frame1, IplImage *frame2) {
        IplImage* diff;
        IplImage* image2;
        IplImage* image1;
        image1 = cvCreateImage(cvGetSize(frame1), frame1->depth, 1);
        image2 = cvCreateImage(cvGetSize(frame1), frame1->depth, 1);
        diff = cvCreateImage(cvGetSize(frame1), frame1->depth, 1);
        cvCvtColor(frame1, image1, CV_BGR2GRAY);
        cvCvtColor(frame2, image2, CV_BGR2GRAY);

        cvAbsDiff(image1, image2, diff);
        cvThreshold(diff, diff, constants::threshold, 255, CV_THRESH_BINARY);
        return diff;
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
        }
        return minLoc;
    }

    int Analysis::inWhichAreaIsMoving(IplImage* curr, IplImage* prev, Presentation* prs) {
        int area;

        CvPoint point[] = {prs->leftUL, prs->img1UL, prs->img2UL, prs->img3UL, prs->rightUL};
        int width[] = {prs->leftBR.x-prs->leftUL.x, prs->img1BR.x-prs->img1UL.x,
        prs->img2BR.x-prs->img2UL.x, prs->img3BR.x-prs->img3UL.x, prs->rightBR.x-prs->rightUL.x};
        int height[] = {prs->leftBR.y-prs->leftUL.y, prs->img1BR.y-prs->img1UL.y,
        prs->img2BR.y-prs->img2UL.y, prs->img3BR.y-prs->img3UL.y, prs->rightBR.y-prs->rightUL.y};

        for (int i = 0; i < 5; i++) {
            cvSetImageROI(curr, cvRect(point[i].x, point[i].y, width[i], height[i]));
            cvSetImageROI(prev, cvRect(point[i].x, point[i].y, width[i], height[i]));
            IplImage* diff = Analysis::getDiff(curr, prev);
            cvResetImageROI(curr);
            cvResetImageROI(prev);
            if (isMoving(diff)) {
                cvReleaseImage(&diff);
                return area = i;
            }
            cvReleaseImage(&diff);
        }

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
        //testScreen shoul be a big rectangle of blue color with black square in upper-left corner and a red one in bottom-right
        //IplImage* testScreen = cvLoadImage("~/testscreen.jpg", CV_LOAD_IMAGE_UNCHANGED);
        //blackSquare should match size of square in upper left of testScreen
        IplImage* blackSquare = cvLoadImage("blackSquare.jpg", CV_LOAD_IMAGE_UNCHANGED);
        //redSquare should match size of square in bottom right of testScreen
        IplImage* redSquare = cvLoadImage("redSquare.jpg", CV_LOAD_IMAGE_UNCHANGED);

        //prs->putImage(cvPoint(0, 0), cvPoint(prs->getScreenWidth(), prs->getScreenHeight()), testScreen);
        //prs->applyBuffer();
        IplImage* frame = cvQueryFrame(cpt->getCapture());
        CvPoint ul = Analysis::getLocation(frame, blackSquare, true);
        CvPoint br = Analysis::getLocation(frame, redSquare, false);

        //set camera resolution depending on captured frame
        cpt->screenHeight = frame->height;
        cpt->screenWidth = frame->width;

        prs->leftOffset = ul.x / cpt->screenWidth;
        prs->rightOffset = (prs->getScreenWidth() - br.x) / cpt->screenWidth;
        prs->topOffset = ul.y / cpt->screenHeight;
        prs->bottomOffset = (prs->getScreenHeight() - br.y) / cpt->screenHeight;

       /* Each image occupies 17% of scroller height and 67% of scroller lenght
         * Each arrow occupies 7% of scroller height and 50% of scroller lenght
         * Space between images and images and arrows - 7%
         */
        //calculations of relative coordinates

        //@todo export those magic numbers to constants and recalc them
        CvPoint upperLeft = cvPoint(0,0);
        CvPoint lowerRight = cvPoint(400,200);
        
        int scrollerWidth = lowerRight.x - upperLeft.x;
        int scrollerHeight = lowerRight.y - upperLeft.y;

        prs->scrollerUL[0] = cvPoint(upperLeft.x + round(0.05*scrollerWidth),
                upperLeft.y + round(0.25*scrollerHeight));
        prs->scrollerBR[0] = cvPoint(upperLeft.x + round(0.12*scrollerWidth),
                upperLeft.y + round(0.75*scrollerHeight));

        prs->scrollerUL[1] = cvPoint(upperLeft.x + round(0.18*scrollerWidth),
                upperLeft.y + round(0.17*scrollerHeight));
        prs->scrollerBR[1] = cvPoint(upperLeft.x + round(0.35*scrollerWidth),
                upperLeft.y + round(0.84*scrollerHeight));

        prs->scrollerUL[2] = cvPoint(upperLeft.x + round(0.41*scrollerWidth),
                upperLeft.y + round(0.17*scrollerHeight));
        prs->scrollerBR[2] = cvPoint(upperLeft.x + round(0.58*scrollerWidth),
                upperLeft.y + round(0.84*scrollerHeight));

        prs->scrollerUL[3] = cvPoint(upperLeft.x + round(0.65*scrollerWidth),
                upperLeft.y + round(0.17*scrollerHeight));
        prs->scrollerBR[3] = cvPoint(upperLeft.x + round(0.82*scrollerWidth),
                upperLeft.y + round(0.84*scrollerHeight));

        prs->scrollerUL[4] = cvPoint(upperLeft.x + round(0.88*scrollerWidth),
                upperLeft.y + round(0.25*scrollerHeight));
        prs->scrollerBR[4] = cvPoint(upperLeft.x + round(0.95*scrollerWidth),
                upperLeft.y + round(0.75*scrollerHeight));
        //formulas here for capture..
        //coeff = camW*(1-left-right)/projW

        //cvReleaseImage(&frame);
        
    }
}