/* 
 * File:   application.hpp
 * Author: fishi
 *
 * Created on Piatok, 2011, marec 18, 9:06
 */

#ifndef APPLICATION_HPP
#define	APPLICATION_HPP

#include "handler.hpp"
#include "capture.hpp"
#include "presentation.hpp"

namespace iwb {
    /**
     * Class representing the whole application.
     */
    class Application {
    public:
        /**
         * Initializes the application.
         * @param argc
         * @param argv
         * @return result, 0 if ok
         */
        int initialize(int argc, char* argv[]);
        int run();
    protected:
        Handler* hndl;
        Capture* cpt;
        Presentation* prs;
    };
}

#endif	/* APPLICATION_HPP */
