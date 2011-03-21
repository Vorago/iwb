/* 
 * File:   touchable.hpp
 * Author: fishi, cers
 *
 * Created on Štvrtok, 2011, marec 17, 16:01
 */

#ifndef TOUCHABLE_HPP
#define	TOUCHABLE_HPP

#include "drawable.hpp"
#include <opencv/cv.h>

// These values need tweaking
#define STD_THRESHOLD 200
#define INTERACTION_TRIGGER 100

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
        void (*action)(void);
        int interaction;
        int threshold;
    public:
        Touchable(char* imagePath, CvPoint projectorUL, CvPoint projectorBR, 
                  CvPoint cameraUL, CvPoint cameraBR, void (*action)(void), int threshold=STD_THRESHOLD);
        ~Touchable();
        void detectTouch(IplImage* mask);
    };
}

#endif	/* TOUCHABLE_HPP */

