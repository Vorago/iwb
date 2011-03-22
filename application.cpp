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

#include <QtGui/QApplication>

namespace iwb {

    Application::~Application() {
        // TODO: make sure that order used doesn't lead to memory leaks
        delete(cpt);
        delete(scroller);
        delete(hndl);
        delete(prs);
    }

    int Application::initialize(int argc, char* argv[]) {
        QApplication a (argc, argv);

        hndl = new Handler();
        cpt = NULL;

        if (!hndl->handleArguments(argc, argv, &cpt, 0, 0)) return -1;

        prs = new Presentation();
        Camera::getInstance()->calibrate(cpt, prs);
        scroller = new Scroller(prs, hndl);
//        Confirmation::create(prs, hndl);

        return 0;
    }

    int Application::run() {
    int i;
        IplImage *cf = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
        for (i=0; i<100; i++)
            cf = cvQueryFrame(cpt->getCapture());
        cpt->saveFrame("bgcapt.jpg", cf);
        cvWaitKey(5000);
        cf = cvQueryFrame(cpt->getCapture());
        cpt->saveFrame("bgcapt2.jpg", cf);

        IplImage *bg = cvCloneImage(cf);
        CvMat bgmat = cv::cvarrToMat(bg);
        const char* winFrame = "winFrame";
	    cvNamedWindow(winFrame, CV_WINDOW_AUTOSIZE);
        CvMat cfmat;

        CvMat *mask = cvCreateMat(bg->height, bg->width, CV_8UC1);
        cvSet(mask, cvScalar(1));
        IplImage *dst_img = cvCreateImage(cvGetSize(bg), bg->depth, bg->nChannels),
                 *gs = cvCreateImage(cvGetSize(bg), IPL_DEPTH_8U, 1),
                 *blur = cvCreateImage(cvGetSize(bg), bg->depth, bg->nChannels),
                 *bw = cvCreateImage(cvGetSize(bg), IPL_DEPTH_8U, 1);
        while (true) {
            cf = cvQueryFrame(cpt->getCapture());
            cfmat = cv::cvarrToMat(cf);
            cvSub(bg, cf, dst_img, NULL);
            cvSmooth(dst_img, blur, CV_BLUR, 5);
            CvPoint o, p;
            o.x = 0;
            o.y = 0;
            p.x = prs->getScreenWidth();
            p.y = prs->getScreenHeight();
            cvCvtColor(blur, gs,CV_RGB2GRAY);
            cvThreshold(gs,bw,128,255,CV_THRESH_TRUNC/*|CV_THRESH_OTSU*/);
            cvThreshold(bw,gs,12,255,CV_THRESH_BINARY/*|CV_THRESH_OTSU*/);
            cvCvtColor(gs,dst_img,CV_GRAY2RGB);
            prs->putImage(o,p,NULL,NULL,dst_img);
            prs->applyBuffer();
            cvShowImage(winFrame, cf);

            prs->drawComponents();
            hndl->detectTouchedComponents(gs);

            cvWaitKey(5);
        }

        delete(scroller);
        cvReleaseImage(&dst_img);
        cvReleaseImage(&bw);
        cvReleaseImage(&gs);
        cvReleaseImage(&blur);
        return 0;
    }
}
