#include <cstdio>
#include <stdio.h>
#include "include/capture.hpp"
#include "include/analysis.hpp"
#include "include/presentation.hpp"
#include "include/scroller.hpp"
#include "include/handler.hpp"
#include "include/camera.hpp"
#include "include/application.hpp"

using namespace iwb;

void scroller(Presentation* prs) {
    /*
    // ====Scroller code start
    //TODO: make a function which will implement a calculation of imgIndexes
    // at the moment works only with three and more images in directory tmp/1
    IplImage *leftArrow = cvLoadImage("res/left.jpg", CV_LOAD_IMAGE_UNCHANGED);
    IplImage *rightArrow = cvLoadImage("res/right.jpg", CV_LOAD_IMAGE_UNCHANGED);
    Scroller scroller;
    //here are loaded all files which are in directory tmp/1
    scroller.loadFileNames();
    scroller.currentImg = 1;
    int movingArea;
    //default indexes - we'll show the first three pictures from array
    int imgIndex1 = 0, imgIndex2 = 1, imgIndex3 = 2;
    prs->drawScroller(prs, scroller.imgArray[imgIndex1], scroller.imgArray[imgIndex2],
            scroller.imgArray[imgIndex3], leftArrow, rightArrow);
    //begin of cycle - we need to implement this part in for(;;) and to use inWhichAreaIsMoving
    //movingArea = Analysis::inWhichAreaIsMoving(leftArrow, rightArrow, prs);
    movingArea = 4; //0 or 4 - string for tests
    int position = scroller.imgArraySize - scroller.currentImg;
    if (movingArea == 0) { //left
        if (position == scroller.imgArraySize || position == 1) { //cycle
            if (position == 1) {
                imgIndex1 = scroller.currentImg - 1;
                imgIndex2 = scroller.currentImg;
                imgIndex3 = 0;
            } else {
                imgIndex1 = scroller.imgArraySize - 1;
                imgIndex2 = 0;
                imgIndex3 = 1;
            }
        } else { //everything is okay
            imgIndex1 = scroller.currentImg - 1;
            imgIndex2 = scroller.currentImg;
            imgIndex3 = scroller.currentImg + 1;
        }
    } else if (movingArea == 4) { //right
        if (position > 3) { //everything is okay
            imgIndex1 = scroller.currentImg + 1;
            imgIndex2 = scroller.currentImg + 2;
            imgIndex3 = scroller.currentImg + 3;
        } else { //cycle
            if (position == 3) {
                imgIndex1 = scroller.currentImg + 1;
                imgIndex2 = scroller.currentImg + 2;
                imgIndex3 = 0;
            } else if (position == 2) {
                imgIndex1 = scroller.currentImg + 1;
                imgIndex2 = 0;
                imgIndex3 = 1;
            } else {
                imgIndex1 = 0;
                imgIndex2 = 1;
                imgIndex3 = 2;
            }
        }
    }
    if (movingArea == 0 || movingArea == 4) {
        scroller.currentImg = imgIndex1;
        prs->drawScroller(prs, scroller.imgArray[imgIndex1], scroller.imgArray[imgIndex2],
                scroller.imgArray[imgIndex3], leftArrow, rightArrow);
    }
    //end of cycle
    // ====Scroller code end
    /**/
}

int main(int argc, char *argv[]) {
    Application* app = new Application();

    int result = 0;
    result = app->initialize(argc, argv);
    if (result) {
        return result;
    }

    result = app->run();

    return result;
}
