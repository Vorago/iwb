/* 
 * File:   imageFrame.hpp
 * Author: fishi
 *
 * Created on Pondelok, 2011, marec 21, 22:45
 */

#ifndef IMAGEFRAME_HPP
#define	IMAGEFRAME_HPP

#define IDLE                0
#define GETTING_CAPTURE     1
#define WAITING_FOR_CORNERS 2
#define CAPTURED            3
#define DRAWING             4
#define CHANGING_IMAGE      5
#define SAVING_IMAGE        6

#include "drawable.hpp"
#include "capture.hpp"
#include "analysis.hpp"
#include "presentation.hpp"

namespace iwb {
    class ImageFrame : public Drawable {
    public:
        ImageFrame(Capture* cpt, Presentation* prs, Analysis* analysis);
        ~ImageFrame();
        virtual void setImagePath(const char* imagePath);
        void update();
        void checkForCapture();
        virtual void draw(Presentation* prs);
        void saveFrame();
    private:
        Capture* cpt;
        Analysis* analysis;
        CvPoint cameraUL;
        CvPoint cameraBR;
        IplImage* ulImage;
        IplImage* brImage;
        IplImage* capturedFrame;
        int state;
        int captureState;
        int currentProcess;
        int startTime;

        void refreshCornerCoords(IplImage* currentFrame);
        void captureFrame(IplImage* currentFrame);
        void checkForMovement();
    };
}

#endif	/* IMAGEFRAME_HPP */

