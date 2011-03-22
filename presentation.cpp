/* 
 * File:   presentation.cpp
 * Author: vorago
 * 
 * Created on December 29, 2010, 11:42 PM
 */


#include "include/presentation.hpp"
#include <stdio.h>

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>


namespace iwb {

    Presentation::Presentation() {
        this->screenHeight = QApplication::desktop()->height();
        this->screenWidth = QApplication::desktop()->width();

        this->window = new MainWindow;
        this->window->show();
        this->window->showFullScreen();

        buffer = cvCreateImage(cvSize(screenWidth, screenHeight), IPL_DEPTH_8U, 3);
        slide = cvCreateImage(cvSize(screenWidth, screenHeight), IPL_DEPTH_8U, 3);

        this->leftOffset = 10;
        this->rightOffset = 10;
        this->topOffset = 10;
        this->bottomOffset = 10;
        
    }

    Presentation::~Presentation() {
        cvReleaseImage(&buffer);
        cvReleaseImage(&slide);
        delete (this->window);

    }

    /* TODO: Check that the image really fits after calculating the new position */
    void Presentation::putImage(CvPoint upperLeft, CvPoint lowerRight,
             CvPoint *OutupperLeft, CvPoint *OutlowerRight, IplImage* image) {
        //calculations of new images coodrinates with respect to offset

        int width = (lowerRight.x - upperLeft.x) -
                         (int) ((double) (leftOffset+rightOffset)*(double)(this->screenWidth)/(double)(image->width));
        int height = (lowerRight.y - upperLeft.y) -
                         (int) ((double) (topOffset+bottomOffset)*(double)(this->screenHeight)/(double)image->height);

        /* Point = OldPoint + (oldsize-newsize)/2 + offset */
        upperLeft.x = upperLeft.x + leftOffset;
        upperLeft.y = upperLeft.y + bottomOffset;


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

        if (OutupperLeft != NULL){
            OutupperLeft->x = upperLeft.x;
            OutupperLeft->y = upperLeft.y;
        }

        if (OutlowerRight != NULL){
            OutlowerRight->x = upperLeft.x + width;
            OutlowerRight->y = upperLeft.y + height;
        }


    }

    /* It doesn't take in account the offsets */
    void Presentation::clearArea(CvPoint upperLeft, CvPoint lowerRight) {
        CvScalar color = CV_RGB(0, 0, 0);
        cvRectangle(buffer, upperLeft, lowerRight, color, CV_FILLED, 0, 0);
    }

     void Presentation::applyBuffer() {
        cvCopy(buffer, slide);

        /* WARNING:
         * I've considered that we'll always use the same settings
         * to capture the image (from above):
         * IPL_DEPTH_8U with 3 channels
         * If not: look for a good way to calculate here:
         * http://www.qtcentre.org/threads/11655-OpenCV-integration
         */
        uchar* qImageBuffer = (uchar *) malloc (slide->width * slide->height * 4 * sizeof (uchar));
        uchar *QImagePtr = qImageBuffer;
        const uchar *iplImagePtr = (const uchar *)this->slide->imageData;
 
        for (int y = 0; y < slide->height; ++y){
          for (int x = 0; x < slide->width; ++x){
            // We cannot help but copy manually.
            QImagePtr[0] = iplImagePtr[0];
            QImagePtr[1] = iplImagePtr[1];
            QImagePtr[2] = iplImagePtr[2];
            QImagePtr[3] = 0;
 
            QImagePtr += 4;
            iplImagePtr += 3;
          }
          iplImagePtr += slide->widthStep - 3 * slide->width;
        }

        QImage *qImage = new QImage (qImageBuffer, slide->width, slide->height, QImage::Format_RGB32);
        this->window->qimage = qImage;
        this->window->update();
        // free(qImageBuffer);

        //        cvShowImage(winPresentFrame, slide);
      }

     int Presentation::getScreenWidth(){
         return screenWidth;
     }

     int Presentation::getScreenHeight(){
         return screenHeight;
     }

    void Presentation::drawScroller(const char *image1,  const char *image2,
            const char *image3, IplImage* leftArrow, IplImage *rightArrow){
        char filepath[80];
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", image1);
        IplImage *img1 = cvLoadImage(filepath, CV_LOAD_IMAGE_UNCHANGED);
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", image2);
        IplImage *img2 = cvLoadImage(filepath, CV_LOAD_IMAGE_UNCHANGED);
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", image3);
        IplImage *img3 = cvLoadImage(filepath, CV_LOAD_IMAGE_UNCHANGED);
        /* Each image occupies 17% of scroller height and 67% of scroller lenght
         * Each arrow occupies 7% of scroller height and 50% of scroller lenght
         * Space between images and images and arrows - 7%
         */
        //calculations of relative coordinates
        putImage(this->scrollerUL[0],this->scrollerBR[0], NULL, NULL, leftArrow);
        putImage(this->scrollerUL[1],this->scrollerBR[1], NULL, NULL, img1);
        putImage(this->scrollerUL[2],this->scrollerBR[2], NULL, NULL, img2);
        putImage(this->scrollerUL[3],this->scrollerBR[3], NULL, NULL, img3);
        putImage(this->scrollerUL[4],this->scrollerBR[4], NULL, NULL, rightArrow);

        applyBuffer();
     }

    void Presentation::drawConfirmation() {

        IplImage *yesButton = cvLoadImage("res/left.jpg", CV_LOAD_IMAGE_UNCHANGED);
        IplImage *noButton = cvLoadImage("res/right.jpg", CV_LOAD_IMAGE_UNCHANGED);

//        putImage(cvPoint(10, 10), cvPoint(50, 50), yesButton);
//        putImage(cvPoint(60, 10), cvPoint(110, 50), noButton);
//        putImage(cvPoint(scrollerUL[0].x, scrollerUL[0].y), cvPoint(scrollerBR[0].x, scrollerBR[0].y), yesButton);
//        putImage(cvPoint(scrollerUL[1].x, scrollerUL[1].y), cvPoint(scrollerBR[1].x, scrollerBR[1].y), noButton);
        printf("ul: %d,%d , br: %d,%d", confirmationUL[0].x,  confirmationUL[0].y,  confirmationBR[0].x,  confirmationBR[0].y);

        putImage(confirmationUL[0],confirmationBR[0], NULL, NULL, yesButton);
        putImage(confirmationUL[1], confirmationBR[1], NULL, NULL, noButton);


        applyBuffer();
    }

    void Presentation::drawComponents() {
        for (std::list<Drawable*>::iterator component = components.begin(); component != components.end(); component++) {
            (*component)->draw(this);
        }

        applyBuffer();
    }

    void Presentation::addComponent(Drawable* component) {
        components.push_back(component);
    }

    void Presentation::removeComponent(Drawable* component) {
        components.remove(component);
    }
}
