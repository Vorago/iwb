#include "include/scroller.hpp"
namespace iwb {

    Scroller::Scroller() {
        imgArraySize = 0;
        imgArray = NULL;
        currentImg = 0;
    }

    Scroller::~Scroller() {

    }

    int Scroller::loadFileNames(){

        /* TODO:
         * reimpliment using scandir
         * make directory name variable
         */
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

}
