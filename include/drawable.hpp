/* 
 * File:   drawable.hpp
 * Author: fishi
 *
 * Created on Štvrtok, 2011, marec 17, 15:50
 */

#ifndef DRAWABLE_HPP
#define	DRAWABLE_HPP

#include <opencv/cv.h>
namespace iwb {
    class Drawable;
}
#include "presentation.hpp"


namespace iwb {
    /**
     * Base class for everything that can be drawn on the whiteboard.
     */
    class Drawable {
    protected:
        IplImage* image;
//        CvPoint projectorUL;
//        CvPoint projectorBR;
        int getProjectorWidth();
        int getProjectorHeight();
        Presentation *prs;
    public:
        CvPoint projectorUL;
        CvPoint projectorBR;
        Drawable(const char* imagePath, Presentation *prs, CvPoint projectorUL, CvPoint projectorBR);
        ~Drawable();

        virtual void setImagePath(const char* imagePath);
        
        /**
         * Method that is automatically called after instantiation
         * of the class. It can be used for initialization.
         */
//        virtual void initialize();

        /**
         * Method used for drawing on the projector.
         * @param prs
         */
        virtual void draw(Presentation* prs);
    };
}

#endif	/* DRAWABLE_HPP */

