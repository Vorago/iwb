#include <cstdlib>
#include "include/capture.hpp"
#include "include/analysis.hpp"
using namespace iwb;

int main(int argc, char *argv[]) {
    int cam;
    Capture* cpt = 0;

    // Argument handling block
    if (argc == 2) {
        char *endptr;
        cam = strtol(argv[1], &endptr, 10);
        if (*endptr == '\0') {
            cpt = new Capture(cam);
        } else {
            cpt = new Capture(argv[1]);
        }
    } else {
        cam = 0;
        cpt = new Capture(cam);
    }

    cpt->showDiff();
    return 0;
}
