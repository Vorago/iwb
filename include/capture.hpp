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
    public:
        CvCapture* capture;
        Capture();
        virtual ~Capture();
        void saveFrame();
    };
}

#endif	/* CAPTURE_HPP */

