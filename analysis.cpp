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
        cvReleaseImage(&image1);
        cvReleaseImage(&image2);
        
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

            cvReleaseImage(&dst_img);
            cvReleaseImage(&bw);
            cvReleaseImage(&blur);

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

}