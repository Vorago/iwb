#include "include/scroller.hpp"
#include "include/camera.hpp"
#include <boost/bind.hpp>

namespace iwb {

    Scroller::Scroller(Presentation* prs, Handler* hndl, ImageFrame* imageFrame) {
        this->prs = prs;
        this->hndl = hndl;
        this->imageFrame = imageFrame;
        imageToBeDisplayed = NULL;
        imgArraySize = 0;
        imgArray = NULL;
        currentImg = 0;
        initialize();
    }

    Scroller::~Scroller() {
        freeFileNameArray();
        freeButtons();

        free(imageToBeDisplayed);
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

    void Scroller::freeButtons() {
        for (int i = 0; i < 5; i++) {
            if (buttons[i] != NULL) {
                deleteButton(buttons[i]);
            }
        }
    }

    void Scroller::deleteButton(Touchable* button) {
        //FIXME move to deconstructors of touchable and drawable
        //hndl->removeComponent(button);
        prs->removeComponent(button);
        delete(button);
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

	void testcb() {
		printf("TOUCHED!\n");
	};

    void touchme() {
        printf("TOUCHED LEFT!\n");
    }

    void Scroller::initialize() {

//        Scroller::instance = this;

        for (int i = 0; i < 5; i++) {
            buttons[i] = NULL;
        }
                
        CvPoint projectorUL[5];
        CvPoint projectorBR[5];

        CvPoint upperLeft = cvPoint(0, 0);
        CvPoint lowerRight = cvPoint(800, 200);

        int scrollerWidth = lowerRight.x - upperLeft.x;
        int scrollerHeight = lowerRight.y - upperLeft.y;

        projectorUL[LEFT_ARROW] = cvPoint(
                upperLeft.x + round(0.05 * scrollerWidth),
                upperLeft.y + round(0.25 * scrollerHeight)
                );
        projectorBR[LEFT_ARROW] = cvPoint(
                upperLeft.x + round(0.12 * scrollerWidth),
                upperLeft.y + round(0.75 * scrollerHeight)
                );

        projectorUL[LEFT_IMAGE] = cvPoint(
                upperLeft.x + round(0.17 * scrollerWidth),
                upperLeft.y + round(0.17 * scrollerHeight)
                );
        projectorBR[LEFT_IMAGE] = cvPoint(
                upperLeft.x + round(0.34 * scrollerWidth),
                upperLeft.y + round(0.84 * scrollerHeight)
                );

        projectorUL[MIDDLE_IMAGE] = cvPoint(
                upperLeft.x + round(0.41 * scrollerWidth),
                upperLeft.y + round(0.17 * scrollerHeight)
                );
        projectorBR[MIDDLE_IMAGE] = cvPoint(
                upperLeft.x + round(0.58 * scrollerWidth),
                upperLeft.y + round(0.84 * scrollerHeight)
                );

        projectorUL[RIGHT_IMAGE] = cvPoint(
                upperLeft.x + round(0.65 * scrollerWidth),
                upperLeft.y + round(0.17 * scrollerHeight)
                );
        projectorBR[RIGHT_IMAGE] = cvPoint(
                upperLeft.x + round(0.82 * scrollerWidth),
                upperLeft.y + round(0.84 * scrollerHeight)
                );

        projectorUL[RIGHT_ARROW] = cvPoint(
                upperLeft.x + round(0.88 * scrollerWidth),
                upperLeft.y + round(0.25 * scrollerHeight)
                );
        projectorBR[RIGHT_ARROW] = cvPoint(
                upperLeft.x + round(0.95 * scrollerWidth),
                upperLeft.y + round(0.75 * scrollerHeight)
                );
        char* paths[MIDDLE_IMAGE] = {"res/left.jpg", "res/right.jpg"};
//        paths[LEFT_ARROW] = "res/left.jpg";
//        paths[LEFT_IMAGE] = "res/right.jpg";
        // TODO: add image paths
        
        buttons[LEFT_ARROW] = new Touchable(paths[0], prs, hndl, projectorUL[LEFT_ARROW], projectorBR[LEFT_ARROW], boost::bind(&iwb::Scroller::shiftLeft, this));

        buttons[RIGHT_ARROW] = new Touchable(paths[1], prs, hndl, projectorUL[RIGHT_ARROW], projectorBR[RIGHT_ARROW], boost::bind(&iwb::Scroller::shiftRight, this));

//        for (int i = 0; i < 2; i++) {
//            printf("%s\n", paths[i]);
//            buttons[i] = new Touchable(paths[i], projectorUL[i], projectorBR[i], cameraUL[i], cameraBR[i]);
//            prs->addComponent(buttons[i]);
//
//        }
        loadFileNames();

        // FIXME: this code will crash if there are less than three images in the folder
        char filepath[80];
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[0]);
        buttons[LEFT_IMAGE] = new Touchable(filepath, prs, hndl, projectorUL[LEFT_IMAGE], projectorBR[LEFT_IMAGE], boost::bind(&iwb::Scroller::handleLeftImageTouch, this));

        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[1]);
        buttons[MIDDLE_IMAGE] = new Touchable(filepath, prs, hndl, projectorUL[MIDDLE_IMAGE], projectorBR[MIDDLE_IMAGE], boost::bind(&iwb::Scroller::handleMiddleImageTouch, this));

        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[2]);
        buttons[RIGHT_IMAGE] = new Touchable(filepath, prs, hndl, projectorUL[RIGHT_IMAGE], projectorBR[RIGHT_IMAGE], boost::bind(&iwb::Scroller::handleRightImageTouch, this));

        displayImages();
        
    }

    void Scroller::handleLeftImageTouch() {
        printf("--- LEFT IMAGE\n");

        setImageToBeDisplayed(LEFT_IMAGE);
//        printf(">>>>>>>.........DISPLAYING LEFT.........<<<<<< %s\n", imageToBeDisplayed);
    }
    
    void Scroller::handleMiddleImageTouch() {
        printf("--- MIDDLE IMAGE\n");

        setImageToBeDisplayed(MIDDLE_IMAGE);
//        printf(">>>>>>>.........DISPLAYING MIDDLE.........<<<<<< %s\n", imageToBeDisplayed);
    }

    void Scroller::handleRightImageTouch() {
        printf("--- RIGHT IMAGE\n");

        setImageToBeDisplayed(RIGHT_IMAGE);
//        printf(">>>>>>>.........DISPLAYING RIGHT.........<<<<<< %s \n", imageToBeDisplayed);
    }

    void Scroller::setImageToBeDisplayed(int imagePosition) {
        if (!Confirmation::create(prs, hndl, boost::bind(&iwb::Scroller::handleYesButton, this), boost::bind(&iwb::Scroller::handleNoButton, this))) {
            return;
        }
        if (imageToBeDisplayed != NULL) {
            free(imageToBeDisplayed);
        }

        imageToBeDisplayed = (char*)malloc(80 * sizeof(char*));

        getImagePath(imagePosition, imageToBeDisplayed);
    }

    void Scroller::handleYesButton() {
        printf("YEEEEEEEEEEEEEEEEEEEEEEEEES \n");
    }

    void Scroller::handleNoButton() {
        printf("NOOOOOOOOOOOOOOOOOOOOOOOOOO \n");
    }

    void Scroller::getImagePath(int imagePosition, char* path) {
        int scrollerPosition = imagePosition - LEFT_IMAGE;
        int arrayPosition = (currentImg + scrollerPosition) % imgArraySize;

//        char filepath[80];
        sprintf(path, "tmp/1/%s", imgArray[arrayPosition]);

//        return filepath;
    }

    void Scroller::displayImages() {
        int first = currentImg % imgArraySize;
        int second = (currentImg + 1) % imgArraySize;
        int third = (currentImg + 2) % imgArraySize;

        char filepath[80];
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[first]);
        buttons[LEFT_IMAGE]->setImagePath(filepath);
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[second]);
        buttons[MIDDLE_IMAGE]->setImagePath(filepath);
        snprintf(filepath, sizeof(filepath), "tmp/1/%s", imgArray[third]);
        buttons[RIGHT_IMAGE]->setImagePath(filepath);
    }

    void Scroller::shiftLeft() {
        currentImg--;
        if (currentImg < 0) {
            currentImg = imgArraySize - 1;
        }

        displayImages();
    }

    void Scroller::shiftRight() {
        printf("GOT HERE\n");
        currentImg++;
        if (currentImg > imgArraySize - 1) {
            currentImg = 0;
        }

        displayImages();
    }

    void Scroller::displayConfirmation() {
        if (Confirmation::create(prs, hndl, NULL, NULL)) {
            printf("DEBUG: confirmation dialog created!\n");
        }
    }
}
