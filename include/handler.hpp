/* 
 * File:   handler.hpp
 * Author: vorago
 *
 * Created on March 1, 2011, 8:03 PM
 */

#ifndef HANDLER_HPP
#define	HANDLER_HPP

namespace iwb {class Handler;}
#include "capture.hpp"
#include <list>
#include "touchable.hpp"

namespace iwb {

    class Handler {
    private:
        bool saveFlag;
        bool loadFlag;
        std::list<Touchable*> components;
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
		void detectTouchedComponents(IplImage *mask);
		void addComponent(Touchable* component);
		void removeComponent(Touchable* component);
    };

}
#endif	/* HANDLER_HPP */

