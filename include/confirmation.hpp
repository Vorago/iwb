/* 
 * File:   confirmation.hpp
 * Author: fishi
 *
 * Created on Nedeľa, 2011, marec 20, 23:06
 */

#ifndef CONFIRMATION_HPP
#define	CONFIRMATION_HPP

#include "presentation.hpp"
#include "handler.hpp"
#include "touchable.hpp"

namespace iwb {
    class Confirmation {
    public:
        static bool create(Presentation* prs, Handler* hndl);
        static void hide();
        ~Confirmation();
    private:
        static Confirmation* instance;
        Confirmation(Presentation* prs, Handler* hndl);
        Presentation* prs;
        Handler* hndl;
        Touchable* yesButton;
        Touchable* noButton;

        void initialize();
        void freeButtons();
        void deleteButton(Touchable* button);
    };
}

#endif	/* CONFIRMATION_HPP */

