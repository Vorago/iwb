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
        Confirmation(Presentation* prs, Handler* hndl);
        ~Confirmation();
    private:
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

