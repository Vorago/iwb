#include "include/application.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cstdio>
#include <stdio.h>
#include "include/capture.hpp"
#include "include/analysis.hpp"
#include "include/presentation.hpp"
#include "include/scroller.hpp"
#include "include/handler.hpp"
#include "include/camera.hpp"
#include "include/confirmation.hpp"
#include "include/imageFrame.hpp"

#include <QtGui/QApplication>

namespace iwb {
    Application::Application(int argc, char **argv){
        this->a = new QApplication(argc, argv);
    }
    Application::~Application() {
        // TODO: make sure that order used doesn't lead to memory leaks
        delete(cpt);
        delete(scroller);
        delete(hndl);
        delete(prs);
        delete (a);
    }

    int Application::initialize(int argc, char* argv[]) {

        hndl = new Handler();
        cpt = NULL;

        if (!hndl->handleArguments(argc, argv, &cpt, 0, 0)) return -1;

        prs = new Presentation();
        Camera::getInstance()->calibrate(cpt, prs);
        analysis = new Analysis(cpt);
        imageFrame = new ImageFrame(cpt, prs, analysis);
//        imageFrame->setImagePath("res/no.jpg");
        imageFrame->saveFrame();
        prs->addComponent(imageFrame);
        scroller = new Scroller(prs, hndl, imageFrame);
//        Confirmation::create(prs, hndl);

        return 0;
    }

    int Application::run() {
    int i;
    int camWidth = Camera::getInstance()->getWidth();
    int camHeight = Camera::getInstance()->getHeight();
        IplImage *cf = cvCreateImage(cvSize(camWidth, camHeight), IPL_DEPTH_8U, 3);
        IplImage *diff = cvCreateImage(cvSize(camWidth, camHeight), IPL_DEPTH_8U, 3);
        IplImage *gs = NULL;
        cpt->saveFrame("bgcapt.jpg", cf);
//        cvWaitKey(5000);
        cf = cvQueryFrame(cpt->getCapture());
        cpt->saveFrame("bgcapt2.jpg", cf);

//        const char* winFrame = "winFrame";
//        cvNamedWindow(winFrame, CV_WINDOW_AUTOSIZE);
//        cvNamedWindow("test", CV_WINDOW_AUTOSIZE);

        cpt->getCapture();
//        cvWaitKey(3000);
        IplImage* previous = cvCloneImage(cvQueryFrame(cpt->getCapture()));
        IplImage* current;
        IplImage* movementDiff;

        int threshold = 1;
        bool backgroundChanged = false;
        while(true) {
            current = cvCloneImage(cvQueryFrame(cpt->getCapture()));
            movementDiff = analysis->getCornerDiff(previous, current);
            cvSaveImage("movementDiff.jpg", movementDiff);
//            int c = 0;
//            for (int x=0; x<movementDiff->width && !backgroundChanged; x++)
//            for (int y=0; y<movementDiff->height && !backgroundChanged; y++) {
//                int elem = CV_IMAGE_ELEM( movementDiff, uchar, y, x);
//                if (elem) {
//                    printf(" %d ", elem);
//                    c++;
//
//                }
//            }
//                    if (c < threshold) {
//                        analysis->refreshBackground();
//                        backgroundChanged = true;
//                        printf("#### CHANGING BACKGROUND #####\n");
//                    }
            imageFrame->update();
            cf = cvQueryFrame(cpt->getCapture());
            gs = analysis->getDiff();
            cvCvtColor(gs, diff, CV_GRAY2RGB);
//            cvShowImage(winFrame, diffs);
//            prs->drawDiff(diff);

//            prs->putImage(cvPoint(0,0), cvPoint(prs->getScreenWidth(), prs->getScreenHeight()), NULL, NULL, analysis->bg);
            prs->drawComponents();

            // TODO: somehow get this gs from somewhere
            hndl->detectTouchedComponents(gs);
            cvReleaseImage(&gs);
            cvWaitKey(5);

            cvReleaseImage(&previous);

            previous = current;
            backgroundChanged = false;
        }


            cvReleaseImage(&previous);
            cvReleaseImage(&current);
            cvReleaseImage(&movementDiff);

//        delete(scroller);
        delete(analysis);
        return 0;
    }
}
