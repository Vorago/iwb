#include "include/scroller.hpp"
#include "include/camera.hpp"

namespace iwb {

    Scroller::Scroller(Presentation* prs, Handler* hndl) {
        imgArraySize = 0;
        imgArray = NULL;
        currentImg = 0;
        initialize(prs, hndl);
    }

    Scroller::~Scroller() {
        freeFileNameArray();
    }

    void Scroller::freeFileNameArray(){
        for (int i = 0; i < imgArraySize; i++) {
            free(imgArray[i]);
        }
        free(imgArray);
        imgArray = NULL;
        imgArraySize = 0;
        currentImg = 0;
    }
    int Scroller::loadFileNames(){

        /* TODO:
         * reimpliment using scandir
         * make directory name variable
         */
        //check in case of loadFileNames was already called
        if(imgArray != NULL)
            freeFileNameArray();
        DIR *dir;
        struct dirent *ent;
        dir = opendir ("tmp/1");
        if (dir != NULL) {
            while ((ent = readdir (dir)) != NULL) {
                    imgArraySize++;
            }
            rewinddir(dir);
            imgArraySize = imgArraySize-2; //because we do not need to count . and ..
            imgArray = (char**)malloc(imgArraySize * sizeof(char*));
            for (int i = 0; i < imgArraySize; i++) {
                ent = readdir (dir);
                if(!ent){
                    break;
                }
                if(strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")){
                   imgArray[i] = (char*)malloc(strlen( ent->d_name) + 1);
                   strcpy(imgArray[i], ent->d_name);
                } else {
                    i--;
                }
            }
            closedir (dir);
        } else {
            /* could not open directory */
            return EXIT_FAILURE;
        }
        return 0;
    }

    void Scroller::initialize(Presentation* prs, Handler* hndl) {

        printf("DEBUG: scroller initialized\n");
                
        CvPoint projectorUL[5];
        CvPoint projectorBR[5];
        CvPoint cameraUL[5];
        CvPoint cameraBR[5];;

        CvPoint upperLeft = cvPoint(0, 0);
        CvPoint lowerRight = cvPoint(800, 200);

        int scrollerWidth = lowerRight.x - upperLeft.x;
        int scrollerHeight = lowerRight.y - upperLeft.y;

        projectorUL[0] = cvPoint(
                upperLeft.x + round(0.05 * scrollerWidth),
                upperLeft.y + round(0.25 * scrollerHeight)
                );
        projectorBR[0] = cvPoint(
                upperLeft.x + round(0.12 * scrollerWidth),
                upperLeft.y + round(0.75 * scrollerHeight)
                );

        projectorUL[1] = cvPoint(
                upperLeft.x + round(0.17 * scrollerWidth),
                upperLeft.y + round(0.17 * scrollerHeight)
                );
        projectorBR[1] = cvPoint(
                upperLeft.x + round(0.34 * scrollerWidth),
                upperLeft.y + round(0.84 * scrollerHeight)
                );

        projectorUL[2] = cvPoint(
                upperLeft.x + round(0.41 * scrollerWidth),
                upperLeft.y + round(0.17 * scrollerHeight)
                );
        projectorBR[2] = cvPoint(
                upperLeft.x + round(0.58 * scrollerWidth),
                upperLeft.y + round(0.84 * scrollerHeight)
                );

        projectorUL[3] = cvPoint(
                upperLeft.x + round(0.65 * scrollerWidth),
                upperLeft.y + round(0.17 * scrollerHeight)
                );
        projectorBR[3] = cvPoint(
                upperLeft.x + round(0.82 * scrollerWidth),
                upperLeft.y + round(0.84 * scrollerHeight)
                );

        projectorUL[4] = cvPoint(
                upperLeft.x + round(0.88 * scrollerWidth),
                upperLeft.y + round(0.25 * scrollerHeight)
                );
        projectorBR[4] = cvPoint(
                upperLeft.x + round(0.95 * scrollerWidth),
                upperLeft.y + round(0.75 * scrollerHeight)
                );
        
        Camera* camera = Camera::getInstance();
        int screenWidth = camera->getWidth();
        int screenHeight = camera->getHeight();
        float leftOffset = camera->getLeftOffset();
        float rightOffset = camera->getRightOffset();
        float topOffset = camera->getRightOffset();
        float bottomOffset = camera->getBottomOffset();

        cameraUL[0] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorUL[0].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorUL[0].y
                );
        cameraBR[0] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorUL[0].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorUL[0].y
                );

        cameraUL[1] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorUL[1].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorUL[1].y
                );
        cameraBR[1] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorBR[1].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorBR[1].y
                );

        cameraUL[2] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorUL[2].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorUL[2].y
                );
        cameraBR[2] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorBR[2].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorBR[2].y
                );

        cameraUL[3] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorUL[3].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorUL[3].y
                );
        cameraBR[3] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorBR[3].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorBR[3].y
                );

        cameraUL[4] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorUL[4].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorUL[4].y
                );
        cameraBR[4] = cvPoint(
                leftOffset * screenWidth + ((screenWidth * (1 - leftOffset - rightOffset)) / screenWidth) * projectorBR[4].x,
                topOffset * screenHeight + ((screenHeight * (1 - topOffset - bottomOffset)) / screenHeight) * projectorBR[4].y
                );

        const char* paths[2] = {"res/left.jpg", "res/right.jpg"};
//        paths[0] = "res/left.jpg";
//        paths[1] = "res/right.jpg";
        // TODO: add image paths

        for (int i = 0; i < 2; i++) {
            printf("%s\n", paths[i]);
            buttons[i] = new Touchable(paths[i], projectorUL[i], projectorBR[i], cameraUL[i], cameraBR[i]);
            prs->addComponent(buttons[i]);

        }
    }

    void Scroller::shiftRight() {
        // TODO: to do
    }

    void Scroller::shiftLeft() {
        // TODO: to do
    }
}
