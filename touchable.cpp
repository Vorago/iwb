#include "include/touchable.hpp"

namespace iwb {
    Touchable::Touchable(char* imagePath, CvPoint projectorUL, CvPoint projectorBR, CvPoint cameraUL, CvPoint cameraBR) : Drawable(imagePath, projectorUL, projectorBR) {
        this->cameraUL = cameraUL;
        this->cameraBR = cameraBR;
    }

    Touchable::~Touchable() {
    }

    int Touchable::getCameraWidth() {
        return cameraBR.x - cameraUL.x;
    }

    int Touchable::getCameraHeight() {
        return cameraBR.y - cameraUL.y;
    }
}
