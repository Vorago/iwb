/* 
 * File:   capture.cpp
 * Author: vorago
 * 
 * Created on December 23, 2010, 1:13 PM
 */

#include "include/capture.hpp"
#include "include/analysis.hpp"

#include <stdio.h>

#include <time.h>

namespace iwb {

    CvCapture* Capture::getCapture() {
        return capture;
    }

    IplImage* Capture::getPreviousFrame() {
        return previousFrame;
    }

    void Capture::setPreviousFrame(IplImage* image) {
        if (previousFrame != NULL) cvReleaseImage(&previousFrame);
        previousFrame = image;
    }

    Capture::Capture(const char* filepath) :
    previousFrame(0) {
        capture = cvCaptureFromAVI(filepath);
    }

    Capture::Capture(int num) :
    previousFrame(0) {
        capture = cvCaptureFromCAM(num);
    }

    Capture::~Capture() {
        cvReleaseCapture(&capture);
    }

    void Capture::saveFrame(const char* filepath, IplImage* image) {
        cvSaveImage(filepath, image);
    }

    void Capture::showDiff() {
//        const char* winDiff = "winDiff";
//        const char* winFrame = "winFrame";
//        cvNamedWindow(winDiff, CV_WINDOW_AUTOSIZE);
//        cvNamedWindow(winFrame, CV_WINDOW_AUTOSIZE);
//
//        for (;;) {
//            IplImage* currentFrame = cvQueryFrame(capture);
//
//            if (previousFrame == NULL) {
//                previousFrame = cvCloneImage(currentFrame);
//                continue;
//            }
//            IplImage* diff = Analysis::getDiff(previousFrame, currentFrame);
//            cvShowImage(winFrame, currentFrame);
//            previousFrame = cvCloneImage(currentFrame);
//            cvShowImage(winDiff, diff);
//            cvWaitKey(0);
//            cvReleaseImage(&diff);
//        }
    }
}
