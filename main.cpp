#include "include/application.hpp"

using namespace iwb;

int main(int argc, char *argv[]) {
    Application* app = new Application();

    int result = 0;
    result = app->initialize(argc, argv);
    if (result) {
        return result;
    }

    result = app->run();

    delete(app);

    return result;

}
