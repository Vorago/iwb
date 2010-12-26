/* 
 * File:   analysis.cpp
 * Author: vorago
 * 
 * Created on December 23, 2010, 1:10 PM
 */

#include "include/analysis.hpp"
namespace iwb {
    IplImage*  analysis::getDiff(IplImage *frame1, IplImage *frame2) {
        IplImage* diff;
        diff = cvCreateImage( cvGetSize(frame1), frame1->depth, frame1->nChannels );
        cvAbsDiff(frame1,frame2, diff);

        return diff;
    }
}