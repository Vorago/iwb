/* 
 * File:   touchable.hpp
 * Author: fishi
 *
 * Created on Štvrtok, 2011, marec 17, 16:01
 */

#ifndef TOUCHABLE_HPP
#define	TOUCHABLE_HPP

#include "drawable.hpp"
#include <opencv/cv.h>

namespace iwb {
    /**
     * Base method for everything that can be touched on the whiteboard.
     */
    class Touchable : public Drawable {
    protected:
        CvPoint cameraUL;
        CvPoint cameraBR;
        int getCameraWidth();
        int getCameraHeight();
    public:
        Touchable(const char* imagePath, CvPoint projectorUL, CvPoint projectorBR, CvPoint cameraUL, CvPoint cameraBR);
        ~Touchable();
        //TODO: implement the checking of touch events
    };
}

#endif	/* TOUCHABLE_HPP */

