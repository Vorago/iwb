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

    float Camera::getLeftOffset() {
        return this->leftOffset;
    }

    float Camera::getRightOffset() {
        return this->rightOffset;
    }

    float Camera::getTopOffset() {
        return this->topOffset;
    }

    float Camera::getBottomOffset() {
        return this->bottomOffset;
    }

    void Camera::calibrate(Capture* cpt, Presentation* prs){
//#ifdef NO_CALIBRATION
        printf("DEBUG: camera calibration disabled!\n");

        this->width = 320;
        this->height = 240;
        this->leftOffset = 0;
        this->rightOffset = 0;
        this->topOffset = 0;
        this->bottomOffset = 0;

//        return;
//#endif

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

        while (!patternFound) {
            printf("trying to find chessboard\n");
            frame = cvQueryFrame(cpt->getCapture());
            patternFound = cvFindChessboardCorners(frame, nsquares, corners, &cc,
                                                   CV_CALIB_CB_ADAPTIVE_THRESH | 
                                                   CV_CALIB_CB_FILTER_QUADS | 
                                                   CV_CALIB_CB_FAST_CHECK);
            prs->putImage(cvPoint(0,0), cvPoint(prs->getScreenWidth(), prs->getScreenHeight()), frame);

            prs->applyBuffer();
            cvWaitKey(5);
        }
        //cpt->saveFrame("cbfound.jpg", frame);

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


/*        cvPoint *corners[4];

        corners[0] = cvPoint(0,0);
        corners[1] = cvPoint(0,prs->getScreenHeight());
        corners[2] = cvPoint(prs->getScreenWidth(), prs->getScreenHeight());
        corners[3] = cpPoint(prs->getScreenWidth(), 0);

        IplImage *calibration = cvCreateImage(cvSize(prs->getScreenWidth(), prs->getScreenHeight()), IPL_DEPTH_8U, 3);
        cvPolyFill(calibration, corners, )*/

        
/*
        //testScreen shoul be a big rectangle of blue color with black square in upper-left corner and a red one in bottom-right
        IplImage* blackScreen = cvLoadImage("res/bg.jpg", 1);
        //blackSquare should match size of square in upper left of testScreen

//        IplImage* blackSquare = cvLoadImage("res/CleftCalib.jpg",1);
        IplImage* blackSquare = cvLoadImage("res/CleftCalib.jpg",1);

        //redSquare should match size of square in bottom right of testScreen
//        IplImage* redSquare = cvLoadImage("res/CrightCalib.jpg", 1);
        IplImage* redSquare = cvLoadImage("res/CrightCalib.jpg", 1);

        //redSquare should match size of square in bottom right of testScreen
        IplImage* redSquare2 = cvLoadImage("res/Cright.jpg",0);

          IplImage* blackSquare2 = cvLoadImage("res/Cleft.jpg", 0);

        prs->putImage(cvPoint(0, 0), cvPoint(prs->getScreenWidth(), prs->getScreenHeight()), blackScreen);
        prs->applyBuffer();

        cvWaitKey(500);
        IplImage* frame =NULL;
        IplImage* frame2;

        for(int i=0;i<200;i++){
            if(i==0){
                cvQueryFrame(cpt->getCapture());
                frame = cvQueryFrame(cpt->getCapture());
                frame2=cvCloneImage(frame);
                cpt->saveFrame("f1Copy.jpg", frame2);
            }
            cvQueryFrame(cpt->getCapture());
            if(i==100){
                prs->putImage(cvPoint(10, 10), cvPoint(64, 48), blackSquare);
                prs->putImage(cvPoint(900, 600), cvPoint(950, 650), redSquare);
                prs->applyBuffer();
                cvWaitKey(100);
            }
            if(i==199){
                frame = cvQueryFrame(cpt->getCapture());
            }
        }
        prs->putImage(cvPoint(0, 0), cvPoint(prs->getScreenWidth(), prs->getScreenHeight()), blackScreen);
        prs->applyBuffer();
        cvWaitKey(100);


        frame2 = cvLoadImage("f1Copy.jpg", 1);
        cpt->saveFrame("f1.jpg", frame2);
        cpt->saveFrame("f2.jpg", frame);
        IplImage *diff = Analysis::getDiff(frame2, frame);

        cpt->saveFrame("diff.jpg", diff);
        CvPoint ul = Analysis::getLocation(diff, blackSquare2, true);
        CvPoint br = Analysis::getLocation(diff, redSquare2, false);
        cvReleaseImage(&diff);
        cvReleaseImage(&blackScreen);
        cvReleaseImage(&blackSquare);
        cvReleaseImage(&redSquare);



        printf("%d\n", ul.x);
        printf("%d\n", ul.y);
        printf("--------\n");
        printf("%d\n", br.x);
        printf("%d\n", br.y);
        printf("--------\n");


        //set camera resolution depending on captured frame
        this->height = frame->height;
        this->width = frame->width;
//        printf("%d\n", cpt->screenWidth);
   //     printf("%d\n", cpt->screenHeight);
      //  printf("--------\n");
       // cpt->saveFrame("calibrate.jpg", frame);
        this->leftOffset = ul.x / this->width;
        this->rightOffset = (prs->getScreenWidth() - br.x) / this->width;
        this->topOffset = ul.y / this->height;
        this->bottomOffset = (prs->getScreenHeight() - br.y) / this->height;
        */
    }
}
