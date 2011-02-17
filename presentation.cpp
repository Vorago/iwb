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
        int width = lowerRight.x - upperLeft.x;
        int height = lowerRight.y - upperLeft.y;

        /*
         * We have to find the largest possible size bound by given rectangle
         * (upperLeft and lowerRight) but with respect to original image
         * aspect ratio.
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
        IplImage* resizedImage = cvCreateImage(cvSize(newWidth, newHeight), IPL_DEPTH_8U, 3);
        cvResize(image, resizedImage, CV_INTER_CUBIC);

        //Setting area for new image
        cvSetImageROI(buffer, cvRect(upperLeft.x, upperLeft.y, newWidth, newHeight));
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

}
