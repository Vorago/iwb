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
#include <boost/bind.hpp>

namespace iwb {
    class Confirmation {
    public:
        static bool create(Presentation* prs, Handler* hndl, boost::function<void (void)> yesCb, boost::function<void (void)> noCb);
        static void hide();
        ~Confirmation();
    private:
        static Confirmation* instance;
        Confirmation(Presentation* prs, Handler* hndl, boost::function<void (void)> yesCb, boost::function<void (void)> noCb);
        Presentation* prs;
        Handler* hndl;
        Touchable* yesButton;
        Touchable* noButton;

        void initialize(boost::function<void (void)> yesCb, boost::function<void (void)> noCb);
        void freeButtons();
        void deleteButton(Touchable* button);
    };
}

#endif	/* CONFIRMATION_HPP */

