/* 
 * File:   capture.cpp
 * Author: vorago
 * 
 * Created on December 23, 2010, 1:13 PM
 */

#include "include/capture.hpp"

namespace iwb {
    CvCapture* capture;

    Capture::Capture() {
        capture = cvCaptureFromCAM(0);
    }

    Capture::~Capture() {
        cvReleaseCapture(&capture);
    }

    void Capture::saveFrame() {
        cvSaveImage("frame.jpg", cvQueryFrame(capture));
    }

}
