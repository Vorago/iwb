#include "include/camera.hpp"
#include "include/analysis.hpp"
#include <stdio.h>

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
        return width;
    }

    int Camera::getHeight() {
        return height;
    }

    float Camera::getLeftOffset() {
        return leftOffset;
    }

    float Camera::getRightOffset() {
        return rightOffset;
    }

    float Camera::getTopOffset() {
        return topOffset;
    }

    float Camera::getBottomOffset() {
        return bottomOffset;
    }

    void Camera::calibrate(Capture* cpt, Presentation* prs){

        printf("DEBUG: calibrating camera\n");

        //testScreen shoul be a big rectangle of blue color with black square in upper-left corner and a red one in bottom-right
        IplImage* blackScreen = cvLoadImage("res/bg.jpg", 1);
        //blackSquare should match size of square in upper left of testScreen

//        IplImage* blackSquare = cvLoadImage("res/CleftCalib.jpg",1);
        IplImage* blackSquare = cvLoadImage("res/Cleft.jpg",1);

        //redSquare should match size of square in bottom right of testScreen
//        IplImage* redSquare = cvLoadImage("res/CrightCalib.jpg", 1);
        IplImage* redSquare = cvLoadImage("res/Cright.jpg", 1);

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
        height = frame->height;
        width = frame->width;
//        printf("%d\n", cpt->screenWidth);
   //     printf("%d\n", cpt->screenHeight);
      //  printf("--------\n");
       // cpt->saveFrame("calibrate.jpg", frame);
        leftOffset = ul.x / width;
        rightOffset = (prs->getScreenWidth() - br.x) / width;
        topOffset = ul.y / height;
        bottomOffset = (prs->getScreenHeight() - br.y) / height;
    }
}
