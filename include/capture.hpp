/* 
 * File:   capture.hpp
 * Author: vorago
 *
 * Created on December 23, 2010, 1:13 PM
 */

#ifndef CAPTURE_HPP
#define	CAPTURE_HPP

#include <opencv/highgui.h>
#include <opencv/cv.h>

namespace iwb {

    class Capture {
    private:
        CvCapture* capture;
        IplImage* previousFrame;
    public:

        /**
         * Constructor for capturing from file
         * 
         * @param filepath path to videofile to capture from
         */
        Capture(const char* filepath);
        /**
         * Constructor for capturing from camera
         *
         * @param num number of camera to use (-1 to default)
         */
        Capture(int num);
        virtual ~Capture();
        /**
         * Captures a frame and saves it to file
         *
         * File is named frame.jpg and is saved to project root
         */
        void saveFrame();

        /**
         * Show differencies between previous and current frame (diff)
         */
        void showDiff();
    };
}

#endif	/* CAPTURE_HPP */

