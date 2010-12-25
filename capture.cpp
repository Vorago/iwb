/* 
 * File:   capture.cpp
 * Author: vorago
 * 
 * Created on December 23, 2010, 1:13 PM
 */

#include "include/capture.hpp"
#include "include/analysis.hpp"

namespace iwb {

    Capture::Capture(const char* filepath) :
            previousFrame(0)
    {
        capture = cvCaptureFromAVI(filepath);
    }

    Capture::Capture(int num) :
            previousFrame(0)
    {
        capture = cvCaptureFromCAM(num);
    }

    Capture::~Capture() {
        cvReleaseCapture(&capture);
    }

    void Capture::saveFrame() {
        cvSaveImage("frame.jpg", cvQueryFrame(capture));
    }

    void Capture::showDiff() {
        std::string window_name = "Diffs";
        cv::namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window

        for(;;) {
            IplImage* currentFrame = cvQueryFrame(capture);
            if (previousFrame == 0) {
                previousFrame = currentFrame;
                continue;
            }
            IplImage* diff = analysis::getDiff(previousFrame, currentFrame);
            previousFrame = currentFrame;
            cv::Mat displayedDiff = diff;
            cv::imshow(window_name, displayedDiff);

            cv::waitKey(40);
            cvZero(diff);
        }

    }



}
