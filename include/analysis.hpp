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

class analysis {
public:
    static IplImage* getDiff(IplImage*, IplImage*);
};

#endif	/* ANALYSIS_HPP */

