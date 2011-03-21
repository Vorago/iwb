#include "include/touchable.hpp"
#include <opencv/cv.h>

namespace iwb {
    Touchable::Touchable(const char* imagePath, CvPoint projectorUL, CvPoint projectorBR, CvPoint cameraUL, CvPoint cameraBR, void (*action)(void), int threshold) : Drawable(imagePath, projectorUL, projectorBR) {
        this->cameraUL = CvPoint(cameraUL);
        this->cameraBR = CvPoint(cameraBR);
        this->threshold = threshold;
        this->action = action;
        this->interaction = 0;
    }

    Touchable::~Touchable() {
    }

    int Touchable::getCameraWidth() {
        return this->cameraBR.x - this->cameraUL.x;
    }

    int Touchable::getCameraHeight() {
        return this->cameraBR.y - this->cameraUL.y;
    }
    void Touchable::detectTouch(IplImage* mask) {
        int x,y,c = 0;
        if (this->interaction > 1)
            this->interaction--;
        for (x=this->cameraUL.x; x<this->cameraBR.x; x++)
            for (y=this->cameraUL.y; y<this->cameraBR.y; y++)
                if (CV_IMAGE_ELEM( mask, uchar, y, x)) {
                    c++;
                    if (c >= this->threshold) {
                        this->interaction += 2;
                    }
                    if (this->interaction > INTERACTION_TRIGGER) {
                        if (this->action != NULL)
                            this->action();
                        this->interaction = 0;
                        return;
                    }

                }
    }
}
