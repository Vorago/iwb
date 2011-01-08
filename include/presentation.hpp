/* 
 * File:   presentation.hpp
 * Author: vorago
 *
 * Created on December 29, 2010, 11:42 PM
 */

#ifndef PRESENTATION_HPP
#define	PRESENTATION_HPP

#include <opencv/highgui.h>
#include <opencv/cv.h>

namespace iwb {
    class Presentation {
    private:
        IplImage* slide;
        IplImage* buffer;
        const char* winPresentFrame;
    public:
        /**
         * Constructor for displaying frames in window
         * @param width width of screen
         * @param height height of screen
         */
        Presentation(int width, int height);

        /**
         * Destructor closes window
         */
        virtual ~Presentation();

        /**
         * Function for clearing rectangular area of buffer set by two points
         *
         * @param upperLeft upper left point
         * @param lowerRight lower right point
         */
        void clearArea(CvPoint upperLeft, CvPoint lowerRight);

        /**
         * Function for adding image to buffer
         *
         * @param upperLeft upper left point
         * @param lowerRight lower right point
         * @param image image to put
         */
        void putImage(CvPoint upperLeft, CvPoint lowerRight, IplImage* image);

        /**
         * Function for updating displayed image
         *
         * Updates image being displayed with contents of buffer
         * Changes contents of slide variable accordingly
         */
         void applyBuffer();
    };
}

#endif	/* PRESENTATION_HPP */

