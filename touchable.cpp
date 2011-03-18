#include "include/touchable.hpp"

namespace iwb {
    Touchable::Touchable(CvPoint projectorUL, CvPoint projectorBR, CvPoint cameraUL, CvPoint cameraBR) : Drawable(projectorUL, projectorBR) {
        this->cameraUL = cameraUL;
        this->cameraBR = cameraBR;
    }

    Touchable::~Touchable() {
    }

    int Touchable::getCameraWidth() {
        //TODO: Implement
    }

    int Touchable::getCameraHeight() {
        //TODO: implement
    }
}
