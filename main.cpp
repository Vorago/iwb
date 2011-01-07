#include <cstdio>
#include "include/capture.hpp"
#include "include/analysis.hpp"
#include "include/presentation.hpp"
using namespace iwb;

int main(int argc, char *argv[]) {
    int cam;
    Capture* cpt = 0;
    // Argument handling block
    if (argc >= 2) {
        char *endptr;
        cam = strtol(argv[1], &endptr, 10);
        if (*endptr == '\0') {
            cpt = new Capture(cam);
        } else {
            cpt = new Capture(argv[1]);
        }
    } else {
        cam = 0;
        cpt = new Capture(cam);
    }

    /*
     * Process resolution in second parameter.
     * If none is given, use 800x600.
     */
    int resWidth = 800;
    int resHeight = 600;
    if (argc == 3) {
        char* str = argv[2];
        char* pch;
        pch = strchr(str, 'x');
        if (pch == NULL) {
            printf("Resolution which was expected in the second argument should "
                    "be of the form of 1024x768");
            return -1;
        }
        pch[0] = '\0';
        pch++;
        resWidth = atoi(str);
        resHeight = atoi(pch);
    }

    //Presentation* prs = new Presentation(resWidth, resHeight);
    //IplImage* img = cvLoadImage("/home/alina/template1.jpg", CV_LOAD_IMAGE_UNCHANGED);
    //prs->putImage(cvPoint(100, 100), cvPoint(500, 700), img);

    //Code below is commented intentionally for testing purposes
    //And also in case of your curiosity you can test it
    /*
    //This part draws a rectangle on preuploaded testFrame
    //Upper left corner of rectangle is defined by template1 location in testFrame
    //Lower right corner of rectangle is defined by template2 location in testFrame
    IplImage *img1, *tmp1, *tmp2;
    img1 = cvLoadImage("/home/alex/testFrame.jpg", 0);
    tmp1 = cvLoadImage("/home/alex/template1.jpg", 0);
    tmp2 = cvLoadImage("/home/alex/template2.jpg", 0);
    cvRectangle(img1,
            analysis::getLocation(img1,tmp1,true),
            analysis::getLocation(img1,tmp2,false),
            cvScalar(0, 0, 255, 0), 1, 0, 0);
    cvNamedWindow("reference", CV_WINDOW_AUTOSIZE);
    cvShowImage("reference", img1);

    cvWaitKey(0);
    cvDestroyWindow("reference");
    cvReleaseImage(&img1);
    cvReleaseImage(&tmp1);
    cvReleaseImage(&tmp2);*/
    //cpt->showDiff();
    return 0;
}
