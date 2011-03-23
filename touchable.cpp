#include "include/touchable.hpp"
#include "include/camera.hpp"
#include <opencv/cv.h>

namespace iwb {
    Touchable::Touchable(char* imagePath, Presentation *prs, Handler *hndl, CvPoint projectorUL, CvPoint projectorBR, boost::function<void ()> action, int threshold) : Drawable(imagePath, prs, projectorUL, projectorBR) {
        this->cameraUL = this->cfppoint(projectorUL);
        this->cameraBR = this->cfppoint(projectorBR);
        this->threshold = threshold;
        this->action = action;
        this->interaction = 0;
        this->hndl = hndl;
        this->prs = prs;
        hndl->addComponent(this);
    }

    Touchable::~Touchable() {
    }
    
    // calculates the position in camera coords from projector coords
    CvPoint Touchable::cfppoint(CvPoint p) {
        Camera *camera = Camera::getInstance();
        CvPoint po = camera->getProjectorOrigin();
        int pw = camera->getProjectorWidth(),
            ph = camera->getProjectorHeight(),
            w  = this->prs->screenWidth,
            h  = this->prs->screenHeight;
        //printf("\n\nx: po.x(%d) w(%d) pw(%d) p.x(%d)\ny: po.y(%d) h(%d) ph(%d) p.y(%d)\n", po.x, w, pw, p.x, po.y, h, ph, p.y);
        int x = po.x+((float)pw/w)*p.x,
            y = po.y+((float)ph/h)*p.y;
        //printf("(%d, %d)", x,y);
        return cvPoint(x,y);
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
                        printf("DETECTED TOUCH!\n");
                        if (this->action != NULL)
                            this->action();
                        this->interaction = 0;
                        return;
                    }

                }
    }
}
