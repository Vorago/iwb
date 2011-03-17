/* 
 * File:   handler.cpp
 * Author: vorago
 * 
 * Created on March 1, 2011, 8:03 PM
 */

#include "include/handler.hpp"

namespace iwb {

    Handler::Handler() {
    }

    Handler::~Handler() {
    }

    bool* Handler::getLoadFlag() {
        return &loadFlag;
    }

    bool* Handler::getSaveFlag() {
        return &saveFlag;
    }

    void Handler::setLoadFlag() {
        loadFlag = true;
        saveFlag = false;
    }

    void Handler::setSaveFlag() {
        saveFlag = true;
        loadFlag = false;
    }

    bool Handler::handleArguments(int argc, char* argv[], Capture **cpt, int *resWidth, int* resHeight) {
        int cam;

        // Argument handling block
        if (argc >= 2) {
            char *endptr;
            cam = strtol(argv[1], &endptr, 10);
            if (*endptr == '\0') {
                *cpt = new Capture(cam);
            } else {
                *cpt = new Capture(argv[1]);
            }
        } else {
            cam = 0;
            *cpt = new Capture(cam);
        }

        /**
         * Process resolution in second parameter.
         * If none is given, use 800x600.
         */
        if (argc == 3) {
            char* str = argv[2];
            char* pch;
            pch = strchr(str, 'x');
            if (pch == NULL) {
                printf("Resolution which was expected in the second argument should "
                        "be of the form of 1024x768");
                return false;
            }
            pch[0] = '\0';
            pch++;
            *resWidth = atoi(str);
            *resHeight = atoi(pch);
        }

        return true;
    }

}
