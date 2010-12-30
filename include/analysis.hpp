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
    };
}
#endif	/* ANALYSIS_HPP */

