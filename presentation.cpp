/* 
 * File:   presentation.cpp
 * Author: vorago
 * 
 * Created on December 29, 2010, 11:42 PM
 */

#include "include/presentation.hpp"


namespace iwb {

    Presentation::Presentation(int width, int height) {
        buffer = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
        slide = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
        winPresentFrame = "winPresentation";
        cvNamedWindow(winPresentFrame, CV_WINDOW_AUTOSIZE);
        cvShowImage(winPresentFrame, slide);
        screenWidth = width;
        screenHeight = height;
        
    }

    Presentation::~Presentation() {
        cvReleaseImage(&buffer);
        cvReleaseImage(&slide);
        cvDestroyWindow(winPresentFrame);

    }

    void Presentation::putImage(CvPoint upperLeft, CvPoint lowerRight, IplImage* image) {
        int newWidth, newHeight;
        //calculations of new images coodrinates with respect to offset
        upperLeft.x = upperLeft.x - leftOffset*screenWidth;
        upperLeft.y = upperLeft.y - topOffset*screenHeight;
        lowerRight.x = lowerRight.x - rightOffset*screenWidth;
        lowerRight.y = lowerRight.y - bottomOffset*screenHeight;

        int width = lowerRight.x - upperLeft.x;
        int height = lowerRight.y - upperLeft.y;

        /*
         * We have to find the largest possible size bound by given rectangle
         * (upperLeft and lowerRight) but with respect to original image
         * aspect ratio.
         * Do not used at the moment.
         */
        float coeffHeight = (float) height / (float) image->height;
        float coeffWidth = (float) width / (float) image->width;
        if (coeffHeight > coeffWidth) {
            newWidth = int(image->width * coeffWidth);
            newHeight = int(image->height * coeffWidth);
        } else {
            newWidth = int(image->width * coeffHeight);
            newHeight = int(image->height * coeffHeight);
        }
        /*
         * TODO:
         * To shrink a photo, it will generally look best with CV_INTER_AREA
         * interpolation, whereas to enlarge an image, it will generally look
         * best with CV_INTER_CUBIC (slow) or CV_INTER_LINEAR (faster but still
         * looks OK).
         */
        IplImage* resizedImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
        cvResize(image, resizedImage, CV_INTER_CUBIC);

        //Setting area for new image
        cvSetImageROI(buffer, cvRect(upperLeft.x, upperLeft.y, width, height));
        cvZero(buffer);
        cvCopyImage(resizedImage, buffer);
        cvResetImageROI(buffer);

        cvReleaseImage(&resizedImage);
    }

    void Presentation::clearArea(CvPoint upperLeft, CvPoint lowerRight) {
        CvScalar color = CV_RGB(0, 0, 0);
        cvRectangle(buffer, upperLeft, lowerRight, color, CV_FILLED, 0, 0);
    }

     void Presentation::applyBuffer() {
        cvCopy(buffer, slide);
        cvShowImage(winPresentFrame, slide);
    }

     int Presentation::getScreenWidth(){
         return screenWidth;
     }

     int Presentation::getScreenHeight(){
         return screenHeight;
     }

    void Presentation::drawScroller(CvPoint upperLeft, CvPoint lowerRight,
             IplImage* image1, IplImage* image2, IplImage* image3){

        int scrollerWidth = lowerRight.x - upperLeft.x;
        int scrollerHeight = lowerRight.y - upperLeft.y;
        IplImage* leftArrow = cvLoadImage("res/left.jpg", CV_LOAD_IMAGE_UNCHANGED);
        IplImage* rightArrow = cvLoadImage("res/right.jpg", CV_LOAD_IMAGE_UNCHANGED);

        /* Each image occupies 17% of scroller height and 67% of scroller lenght
         * Each arrow occupies 7% of scroller height and 50% of scroller lenght
         * Space between images and images and arrows - 7%
         */
        //calculations of relative coordinates
        leftUL = cvPoint(upperLeft.x + round(0.05*scrollerWidth),
                upperLeft.y + round(0.25*scrollerHeight));
        leftBR = cvPoint(upperLeft.x + round(0.12*scrollerWidth),
                upperLeft.y + round(0.75*scrollerHeight));

        img1UL = cvPoint(upperLeft.x + round(0.18*scrollerWidth),
                upperLeft.y + round(0.17*scrollerHeight));
        img1BR = cvPoint(upperLeft.x + round(0.35*scrollerWidth),
                upperLeft.y + round(0.84*scrollerHeight));

        img2UL = cvPoint(upperLeft.x + round(0.41*scrollerWidth),
                upperLeft.y + round(0.17*scrollerHeight));
        img2BR = cvPoint(upperLeft.x + round(0.58*scrollerWidth),
                upperLeft.y + round(0.84*scrollerHeight));

        img3UL = cvPoint(upperLeft.x + round(0.65*scrollerWidth),
                upperLeft.y + round(0.17*scrollerHeight));
        img3BR = cvPoint(upperLeft.x + round(0.82*scrollerWidth),
                upperLeft.y + round(0.84*scrollerHeight));

        rightUL = cvPoint(upperLeft.x + round(0.88*scrollerWidth),
                upperLeft.y + round(0.25*scrollerHeight));
        rightBR = cvPoint(upperLeft.x + round(0.95*scrollerWidth),
                upperLeft.y + round(0.75*scrollerHeight));

        putImage(cvPoint(leftUL.x, leftUL.y), cvPoint(leftBR.x, leftBR.y), leftArrow);
        putImage(cvPoint(img1UL.x, img1UL.y), cvPoint(img1BR.x, img1BR.y), image1);
        putImage(cvPoint(img2UL.x, img2UL.y), cvPoint(img2BR.x, img2BR.y), image2);
        putImage(cvPoint(img3UL.x, img3UL.y), cvPoint(img3BR.x, img3BR.y), image3);
        putImage(cvPoint(rightUL.x, rightUL.y), cvPoint(rightBR.x, rightBR.y), rightArrow);

        applyBuffer();
     }


}
