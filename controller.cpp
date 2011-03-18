#include "include/controller.hpp"
#include <opencv/cv.h>

namespace iwb {
    Controller::Controller(char* imagePath, CvPoint projectorUL, CvPoint projectorBR, CvPoint cameraUL, CvPoint cameraBR)
            : Touchable(projectorUL, projectorBR, cameraUL, cameraBR) {
        image = cvLoadImage(imagePath, 0);
    }

    Controller::~Controller() {
        
    }

    void Controller::draw(Presentation* prs) {
        
    }
}
