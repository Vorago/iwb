/* 
 * File:   handler.hpp
 * Author: vorago
 *
 * Created on March 1, 2011, 8:03 PM
 */

#ifndef HANDLER_HPP
#define	HANDLER_HPP

#include "capture.hpp"

namespace iwb {

    class Handler {
    private:
        bool saveFlag;
        bool loadFlag;
    public:
        Handler();
        virtual ~Handler();
        bool* getSaveFlag();
        bool* getLoadFlag();
        void setSaveFlag();
        void setLoadFlag();
        void handleSave();
        void handleLoad();
        bool handleArguments(int argc, char* argv[], Capture **cpt, int *resWidth, int* resHeight);
    };

}
#endif	/* HANDLER_HPP */

