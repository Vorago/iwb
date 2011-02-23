/* 
 * File:   scroller.hpp
 * Author: alina
 *
 * Created on February 23, 2011, 11:10 PM
 */


#ifndef SCROLLER_HPP
#define	SCROLLER_HPP

#include <dirent.h>
#include <cstdlib>
#include <cstring>


namespace iwb {
    class Scroller {
    public:
        int imgArraySize;
        char **imgArray;
        int currentImg;

        Scroller();

        virtual ~Scroller();

        int loadFileNames();


    };
}

#endif	/* SCROLLER_HPP */

