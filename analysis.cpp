/* 
 * File:   analysis.cpp
 * Author: vorago
 * 
 * Created on December 23, 2010, 1:10 PM
 */

#include "include/analysis.hpp"
#include "include/constants.hpp"

namespace iwb {

    IplImage* analysis::getDiff(IplImage *frame1, IplImage *frame2) {
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

    CvPoint analysis::getLocation(IplImage *source, IplImage *pattern, bool upperLeft) {
        IplImage* matchRes;
        double minVal, maxVal;
        CvPoint minLoc, maxLoc;
        matchRes = cvCreateImage(cvSize(
                source->width - pattern->width + 1,
                source->height - pattern->height + 1
                ), IPL_DEPTH_32F, 0);

        cvMatchTemplate(source, pattern, matchRes, CV_TM_SQDIFF);
        cvMinMaxLoc(matchRes, &minVal, &maxVal, &minLoc, &maxLoc, 0);
        
        cvReleaseImage(&matchRes);
        if(!upperLeft){
            minLoc.x += pattern->width;
            minLoc.y += pattern->height;
        }
        return minLoc;
    }
}