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
    class analysis {
    public:
        /**
         * Function for frame differentiation
         */
        static IplImage* getDiff(IplImage*, IplImage*);

        /**
         * Function for motion detection
         */
        static bool isMoving(const IplImage* diff);

    };
}
#endif	/* ANALYSIS_HPP */

