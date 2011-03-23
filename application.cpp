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

        analysis = new Analysis(cpt);
        prs = new Presentation();
        Camera::getInstance()->calibrate(cpt, prs);
        imageFrame = new ImageFrame(cpt, prs, analysis);
//        imageFrame->setImagePath("res/no.jpg");
        imageFrame->saveFrame();
        prs->addComponent(imageFrame);
        scroller = new Scroller(prs, hndl);
//        Confirmation::create(prs, hndl);

        return 0;
    }

    int Application::run() {
    int i;
        IplImage *cf = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
        IplImage *diff = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
        IplImage *gs = NULL;
        for (i=0; i<100; i++)
            cf = cvQueryFrame(cpt->getCapture());
        cpt->saveFrame("bgcapt.jpg", cf);
//        cvWaitKey(5000);
        cf = cvQueryFrame(cpt->getCapture());
        cpt->saveFrame("bgcapt2.jpg", cf);

        const char* winFrame = "winFrame";
        cvNamedWindow(winFrame, CV_WINDOW_AUTOSIZE);
//        cvNamedWindow("test", CV_WINDOW_AUTOSIZE);

        cpt->getCapture();
//        cvWaitKey(3000);
        
        while(true) {
            imageFrame->update();
            cf = cvQueryFrame(cpt->getCapture());
            cvShowImage(winFrame, cf);
            gs = analysis->getDiff();
            cvCvtColor(gs, diff, CV_GRAY2RGB);
            prs->drawDiff(diff);
            prs->drawComponents();

            // TODO: somehow get this gs from somewhere
            hndl->detectTouchedComponents(gs);
            cvReleaseImage(&gs);
            cvWaitKey(5);
        }

//        delete(scroller);
        delete(analysis);
        return 0;
    }
}
