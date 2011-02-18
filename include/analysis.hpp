/* 
 * File:   analysis.hpp
 * Author: vorago
 *
 * Created on December 23, 2010, 1:10 PM
 */

#ifndef ANALYSIS_HPP
#define	ANALYSIS_HPP

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include "capture.hpp"
#include "presentation.hpp"

namespace iwb {

    class Analysis {
    public:
        /**
         * Function for frame differentiation
         *
         * @param frame1 first frame for differntiation
         * @param frame2 seconf frame for differentiation
         * @return monochrome frame with difference
         */
        static IplImage* getDiff(IplImage* frame1, IplImage* frame2);

        /**
         * Function for coordinates extraction
         *
         * @param source image for analysing
         * @param pattern image to search for
         * @param upperLeft corner coords to return. True->upperLeft, False->lowerRight
         * @return coordinates of pattern in image
         */
        static CvPoint getLocation(IplImage *source, IplImage *pattern, bool upperLeft);

        /**
         * Function for motion detection
         *
         * @param diff monochrome frame with difference
         * @return true if diff contains white, false otherwise
         */
        static bool isMoving(const IplImage* diff);

        /**
         * Function for motion detection
         *
         * @return -1 if no motion, {0,1,2,3,4} otherwise
         */
        static int inWhichAreaIsMoving(IplImage* curr, IplImage* prev, Presentation* prs);

        /**
         * Function for calibrating projector output by setting offsets
         *
         * requires 3 files:
         * ~/testscreen.jpg
         * ~/blacksquare.jpg
         * ~/redsquare.jpg
         * more info in function internal comments
         *
         * @param cpt Capture class instance for getting picture
         * @param prs Presentation class instance for outputting test picture
         */
        static void doCalibrate(Capture* cpt, Presentation* prs);
    };
}
#endif	/* ANALYSIS_HPP */

