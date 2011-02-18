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
        int screenWidth;
        int screenHeight;
    public:
        float leftOffset;
        float rightOffset;
        float topOffset;
        float bottomOffset;
        CvPoint leftUL, leftBR, img1UL, img1BR, img2UL, img2BR, img3UL, img3BR, rightUL, rightBR;
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
         
         /**
          * Resolution width accessor
          *
          * @return int width
          */
         int getScreenWidth();

         /**
          * Resolution height accessor
          *
          * @return int height
          */
         int getScreenHeight();
         /**
          * Function for drawing scroller with 3 images
          *
          * @param upperLeft upper left point of scroller
          * @param lowerRight lower right point of scroller
          * @param image1 one of three images to put
          * @param image2 one of three images to put
          * @param image3 one of three images to put
          */
         void drawScroller(CvPoint upperLeft, CvPoint lowerRight,
         IplImage* image1, IplImage* image2, IplImage* image3);
    };
}

#endif	/* PRESENTATION_HPP */

