/* 
 * File:   drawable.cpp
 * Author: fishi
 * 
 * Created on Štvrtok, 2011, marec 17, 16:05
 */

#include "include/drawable.hpp"

namespace iwb {
    Drawable::Drawable(CvPoint projectorUL, CvPoint projectorBR) {
        this->projectorUL = projectorUL;
        this->projectorBR = projectorBR;
        
        initialize();
    }

    Drawable::~Drawable() {
    }

    int Drawable::getProjectorWidth() {
        //TODO: Implement
    }

    int Drawable::getProjectorHeight() {
        //TODO: implement
    }

    void Drawable::initialize() {
        printf("DEBUG: drawable initialized\n");
    }
}
