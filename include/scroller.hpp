/*
 * File:   scroller.hpp
 * Author: alina
 *
 * Created on February 23, 2011, 11:10 PM
 */


#ifndef SCROLLER_HPP
#define	SCROLLER_HPP

#define LEFT_ARROW  0
#define RIGHT_ARROW 4

#define LEFT_IMAGE      1
#define MIDDLE_IMAGE    2
#define RIGHT_IMAGE     3

#include <dirent.h>
#include <cstdlib>
#include <cstring>
#include <opencv/cv.h>
#include "touchable.hpp"
#include "handler.hpp"
#include "confirmation.hpp"



namespace iwb {
    class Scroller {
    private:
        Presentation* prs;
        Handler* hndl;
        Touchable* buttons[5];

        /**
         * Frees allocated memory and resets the variables
         */
        void freeFileNameArray();
        void freeButtons();
        void deleteButton(Touchable* button);
        void displayImages();
        
    public:
        int imgArraySize;
        char **imgArray;
        int currentImg;
        /**
         * Constructor initislizes variables
         */
        Scroller(Presentation* prs, Handler* hndl);

        virtual ~Scroller();
        /**
         * Loads all filenames which are in directory tmp/session_id
         * Files are accessible from imgArray field
         */
        int loadFileNames();
        
        void shiftRight();
        void shiftLeft();

        virtual void initialize();

        void displayConfirmation();

    };
}

#endif	/* SCROLLER_HPP */

