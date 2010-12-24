/* 
 * File:   main.cpp
 * Author: vorago
 *
 * Created on December 18, 2010, 12:32 PM
 */
#include <stdlib.h>
#include <stdio.h>
#include "include/capture.hpp"
using namespace iwb;

int main(int argc, char *argv[]) {
    Capture cpt;

    cpt.saveFrame(); // first frame usually is blank, because cam needs time to init
    cpt.saveFrame();
    return 0;
}
