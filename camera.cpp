#include "include/camera.hpp"
#include "include/analysis.hpp"
#include <stdio.h>
#include <vector>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv/cv.h>

namespace iwb {
    Camera* Camera::instance = NULL;

    Camera* Camera::getInstance() {
        if (instance == NULL) {
            instance = new Camera();
        }
        return instance;
    }

    Camera::Camera() {
    }

    int Camera::getWidth() {
        return this->width;
    }

    int Camera::getHeight() {
        return this->height;
    }

    int Camera::getProjectorWidth() {
        return this->projectorWidth;
    }

    int Camera::getProjectorHeight() {
        return this->projectorHeight;
    }

    CvPoint Camera::getProjectorOrigin() {
        return this->projectorOrigin;
    }

    void Camera::calibrate(Capture* cpt, Presentation* prs){
#ifdef NO_CALIBRATION
        printf("DEBUG: camera calibration disabled!\n");

        this->width = 320;
        this->height = 240;
        this->projectorWidth = 320;
        this->projectorHeight = 240;
        this->projectorOrigin = cvPoint(0,0);

        return;
#endif

        printf("DEBUG: calibrating camera\n");

        CvSize nsquares = cvSize(7,5);
        CvPoint2D32f* corners = new CvPoint2D32f[ 7*5 ];
        //IplImage *cb = cvLoadImage("res/chessboard.png",1);

        //IplImage *fake = cvLoadImage("fake.jpg", 1);
        //IplImage *src = cvCreateImage(cvGetSize(fake), IPL_DEPTH_8U, 1);
        //cvCvtColor(fake, src, CV_RGB2GRAY);

        IplImage *frame;
        bool patternFound = false;
        int cc;

        printf("trying to find chessboard");
        while (!patternFound) {
            printf(".");
            frame = cvQueryFrame(cpt->getCapture());
            patternFound = cvFindChessboardCorners(frame, nsquares, corners, &cc,
                                                   CV_CALIB_CB_ADAPTIVE_THRESH | 
                                                   CV_CALIB_CB_FILTER_QUADS | 
                                                   CV_CALIB_CB_FAST_CHECK);
            prs->putImage(cvPoint(0,0), cvPoint(prs->getScreenWidth(), prs->getScreenHeight()), frame);

            prs->applyBuffer();
            cvWaitKey(5);
        }
        printf("\n");
        float x = 2*corners[0].x-corners[1].x,
              y = 2*corners[0].y-corners[7].y;

        this->projectorOrigin = cvPoint((int)x, (int)y);

        x = 2*corners[34].x-corners[33].x;
        y = 2*corners[34].y-corners[27].y;


        this->projectorWidth = (int)(x-this->projectorOrigin.x);
        this->projectorHeight = (int)(y-this->projectorOrigin.y);

        printf("Projector: (%d, %d): %dx%d)\n", this->projectorOrigin.x, this->projectorOrigin.y, this->projectorWidth,this->projectorHeight);

        cpt->saveFrame("cbfound.jpg", frame);

        // improve result (thoug through testing, it seems it makes it worse - disabling)
        // could use simple linear regression to get a "better" calibration.
//        IplImage *frame_gray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
//        cvFindCornerSubPix( frame_gray, corners, cc, cvSize( 11, 11 ), 
//                        cvSize( -1, -1 ), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

        // draw calibration result
        cvDrawChessboardCorners( frame, nsquares , corners, cc, patternFound );
        prs->putImage(cvPoint(0,0), cvPoint(prs->getScreenWidth(), prs->getScreenHeight()), frame);
        prs->applyBuffer();
        // display result longer
        cvWaitKey(5000);



// generate chessboard programatically. (htf does cvFillPoly work?! - using image for now)
//        IplImage *cb = cvCreateImage(cvSize(prs->getScreenWidth(), prs->getScreenHeight()), IPL_DEPTH_8U, 1);
//        int x,y, dx = prs->getScreenWidth()/nsquares.width, dy = prs->getScreenHeight()/nsquares.height;
//        CvPoint *corners[4];
//        for (x=0; x<prs->getScreenWidth(); x+=dx) {
//            for (y=0; y<prs->getScreenHeight(); y+=dy) {
//                corners[0] = cvPoint(x,y);
//                corners[1] = cvPoint(x+dx,y);
//                corners[2] = cvPoint(x+dx,y+dy);
//                corners[3] = cvPoint(x,y+dy);
//                cvFillPoly(...)
//            }
//        }


    }
}
