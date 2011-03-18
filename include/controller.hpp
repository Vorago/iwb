/* 
 * File:   controller.h
 * Author: fishi
 *
 * Created on Piatok, 2011, marec 18, 11:57
 */

#ifndef CONTROLLER_H
#define	CONTROLLER_H

#include "touchable.hpp"
#include <opencv/cv.h>

namespace iwb {
    class Controller : public Touchable {
    private:
        IplImage* image;
    public:
        Controller(char* imagePath, CvPoint projectorUL, CvPoint projectorBR, CvPoint cameraUL, CvPoint cameraBR);
        ~Controller();
        virtual void draw(Presentation* prs);
    };
}

#endif	/* CONTROLLER_H */

