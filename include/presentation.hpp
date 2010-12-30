/* 
 * File:   presentation.hpp
 * Author: vorago
 *
 * Created on December 29, 2010, 11:42 PM
 */

#ifndef PRESENTATION_HPP
#define	PRESENTATION_HPP

#include <opencv/highgui.h>

namespace iwb {
    class Presentation {
    private:
        IplImage* slide;
        IplImage* buffer;
    public:
        /**
         * Constructor for displaying frames in window
         */
        Presentation();

        /**
         * Destructor closes window
         */
        virtual ~Presentation();

        /**
         * Function for clearing rectangular area of buffer set by two points
         *
         * @param p1 upper left point
         * @param p2 lower right point
         */
        void clearArea(CvPoint p1, CvPoint p2);

        /**
         * Function for adding image to buffer
         *
         * @param p1 upper left point
         * @param p2 lower right point
         * @param image image to put
         */
        void putImage(CvPoint p1, CvPoint p2, IplImage* image);

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

