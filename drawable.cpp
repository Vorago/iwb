/* 
 * File:   drawable.cpp
 * Author: fishi
 * 
 * Created on Štvrtok, 2011, marec 17, 16:05
 */

#include "include/drawable.hpp"

namespace iwb {
    Drawable::Drawable(const char* imagePath, CvPoint projectorUL, CvPoint projectorBR) {
        image = cvLoadImage(imagePath, CV_LOAD_IMAGE_UNCHANGED);
        this->projectorUL = projectorUL;
        this->projectorBR = projectorBR;
        
        initialize();
    }

    Drawable::~Drawable() {
        cvReleaseImage(&image);
    }

    int Drawable::getProjectorWidth() {
        return projectorBR.x - projectorUL.x;
    }

    int Drawable::getProjectorHeight() {
        return projectorBR.y - projectorUL.y;
    }

    void Drawable::initialize() {
        printf("DEBUG: drawable initialized\n");
    }

    void Drawable::draw(Presentation* prs) {
        prs->putImage(projectorUL, projectorBR, image);
    }
}
