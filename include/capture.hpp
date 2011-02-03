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
        /**
         * Destructor releases capturing
         */
        virtual ~Capture();
        /**
         * Captures a frame and saves it to file
         *
         * File is named frame.jpg and is saved to project root
         */
        void saveFrame(const char* filepath, IplImage* image);

        /**
         * Show differencies between previous and current frame (diff)
         */
        void showDiff();

        /**
         * Set previous frame
         */
        void setPreviousFrame(IplImage* image);

        /**
         * Get previous frame
         */
        IplImage* getPreviousFrame();

        /**
         * Get capture
         */
        CvCapture* getCapture();
    };
}

#endif	/* CAPTURE_HPP */

