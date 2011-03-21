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

namespace iwb {

    int Application::initialize(int argc, char* argv[]) {
        hndl = new Handler();
        cpt = NULL;

        int resWidth = 1024;
        int resHeight = 768;

        if (!hndl->handleArguments(argc, argv, &cpt, &resWidth, &resHeight)) return -1;

        prs = new Presentation(resWidth, resHeight);
        Camera::getInstance()->calibrate(cpt, prs);
        scroller = new Scroller(prs, hndl);

        return 0;
    }

    int Application::run() {
        IplImage *cf = cvQueryFrame(cpt->getCapture());

        IplImage *bg = cvCloneImage(cf);;
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
            prs->putImage(o,p,dst_img);
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
