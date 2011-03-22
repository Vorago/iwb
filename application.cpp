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

namespace iwb {

    Application::~Application() {
        // TODO: make sure that order used doesn't lead to memory leaks
        delete(cpt);
        delete(scroller);
        delete(hndl);
        delete(prs);
    }

    int Application::initialize(int argc, char* argv[]) {
        hndl = new Handler();
        cpt = NULL;

        int resWidth = 1440;
        int resHeight = 900;

        if (!hndl->handleArguments(argc, argv, &cpt, &resWidth, &resHeight)) return -1;

        analysis = new Analysis(cpt);
        prs = new Presentation(resWidth, resHeight);
        Camera::getInstance()->calibrate(cpt, prs);
        imageFrame = new ImageFrame(cpt, analysis);
//        imageFrame->setImagePath("res/no.jpg");
        imageFrame->saveFrame();
        prs->addComponent(imageFrame);
        scroller = new Scroller(prs, hndl);
//        Confirmation::create(prs, hndl);

        return 0;
    }

    int Application::run() {
        IplImage *cf;
        const char* winFrame = "winFrame";
        cvNamedWindow(winFrame, CV_WINDOW_AUTOSIZE);
//        cvNamedWindow("test", CV_WINDOW_AUTOSIZE);

        cpt->getCapture();
        cvWaitKey(3000);
        
        while(true) {
            imageFrame->update();
            cf = cvQueryFrame(cpt->getCapture());
//            cvRectangle(cf, imageFrame->projectorUL, imageFrame->projectorBR, cvScalar(0, 0, 255, 0), 1, 0, 0);
            cvShowImage(winFrame, cf);

            prs->drawDiff(cvCloneImage(analysis->getDiff()));
            prs->drawComponents();

//            cvShowImage("test", cvCloneImage(analysis->getCornerDiff()));
    //			hndl->detectTouchedComponents(gs);
            cvWaitKey(5);
        }

//        delete(scroller);
        delete(analysis);
        return 0;
    }
}
