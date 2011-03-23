#ifndef CORNERDETECTOR_HPP
#define	CORNERDETECTOR_HPP

#include <iostream>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#define WHITE 255
#define BLACK 0
#define forpix(x,y,src)   for(int y=0; y<src->height; y++) for(int x=0; x<src->width; x++)
#define ford(dx,dy)   for(int dy=-1; dy<2; dy++) for(int dx=-1; dx<2; dx++)
#define forbrd(dx,dy)   for(int dy=0; dy<2; dy++) for(int dx=0; dx<2; dx++)

using namespace std;
namespace iwb {

    class CornerDetector {
    private:
        unsigned char fakePixel;
        int fragmentCount;
    public:

        unsigned char &Pixels(IplImage* src, int x, int y) {
            if (x < 0 || x >= src->width || y < 0 || y >= src->height) return fakePixel;
            return *(unsigned char*) (src->imageDataOrigin + y * src->widthStep + x);
        }

        bool detect(IplImage* src, CvPoint* outLU, CvPoint* outRB) {
            fakePixel = BLACK;
            fragmentCount = 0;

            clean(src);
            inflate(src, WHITE);
            fix(src);
//            inflate(src, WHITE);
//            inflate(src, BLACK);
            
            IplImage* fragment = cvCloneImage(src);
            forpix(x,y,src){
                CvPoint point;
                point.x = x;
                point.y = y;
                CvPoint lu, br;
                if(floodCut(src, fragment, point, lu, br)) {
                    fragmentCount++;
                    if(fragmentCount >2 ) return false;
                    if(fragmentCount == 1){
                        if(!isACorner(fragment, lu, br, true)) return false;
                        outLU->x = lu.x;
                        outLU->y = lu.y;
                    }
                    if(fragmentCount == 2){
                        if(!isACorner(fragment, lu, br, false)) return false;
                        outRB->x = br.x;
                        outRB->y = br.y;
                    }
                    
//                    dump(fragment, lu, br);
//                    Rectangle(fragment, lu, br, 25);
//                    cvWaitKey(100);
//                    cin.get();
                }
            }
            if(fragmentCount != 2) return false;
            return true;
        }

        void dump(IplImage* src, CvPoint ul, CvPoint br){
            cout << "---| Corner recognition start |---" << endl;
            for(int y = ul.y; y<=br.y; y++){
                for(int x = ul.x; x<=br.x; x++){
                    cout << (Pixels(src,x,y) == WHITE ? "[]" : "..");
                }
                cout << endl;
            }
            cout << "---| --- |---" << endl;
        }

        bool isACorner(IplImage* src, CvPoint ul, CvPoint br, bool isUpperLeft){
            int h = br.y - ul.y;
            int w = br.x - ul.x;
            int s = MAX(h,w);
            int* bufA = (int*)malloc(sizeof(int)*s*2);
            int* bufB = (int*)malloc(sizeof(int)*s*2);
            memset(bufA, 0, sizeof(int)*s*2);
            memset(bufB, 0, sizeof(int)*s*2);

            // slashes (first half)
            for(int i=0; i<s; i++){
                int diag = i;
                unsigned char c = BLACK;
                for(int j=0; j<=i; j++){

                    int x = ul.x + j;
                    int y = ul.y + i-j;
                    unsigned char c2 = Pixels(src, x,y);
                    if(c != c2){
                        if(c2 == WHITE){
                            bufA[diag]++;
                        }
                        c = c2;
                    }
                }
            }
            // slashes (second half)
            for(int i=1; i<s; i++){
                int diag = i + s - 1;
                unsigned char c = BLACK;
                for(int j=s; j>=i; j--){
                    int x = ul.x + s-j+i;
                    int y = ul.y + j;
                    unsigned char c2 = Pixels(src, x,y);
                    if(c != c2){
                        if(c2 == WHITE){
                            bufA[diag]++;
                        }
                        c = c2;
                    }
                }
            }

            // backslashes (first half)
            for(int i=0; i<=s; i++){
                int diag = i;
                unsigned char c = BLACK;
                for(int j=0; j<=i; j++){
                    int x = ul.x + j;
                    int y = ul.y + s-i+j-1;
                    unsigned char c2 = Pixels(src, x,y);
                    if(c != c2){
                        if(c2 == WHITE){
                            bufB[diag]++;
                        }
                        c = c2;
                    }
                }
            }

            //backslashes (second half)
            for(int i=1; i<s; i++){
                int diag = s+i;
                unsigned char c = BLACK;
                for(int j=0; j<s-i; j++){
                    int x = ul.x + i+j;
                    int y = ul.y + j;
                    unsigned char c2 = Pixels(src, x,y);
                    if(c != c2){
                        if(c2 == WHITE){
                            bufB[diag]++;
                        }
                        c = c2;
                    }
                }
            }

            if(isUpperLeft){
                //check for UL corner
            }else{
                //check for BR corner
            }

//            for(int i=0; i<s*2-1; i++)
//                cout << "i: " << i << " B: " << bufB[i] << endl;
//            cvSaveImage("lookatme.jpg", src);
            
            free(bufA);
            free(bufB);

            return true;
        }

        void Rectangle(IplImage *src, CvPoint lu, CvPoint br, unsigned char color){
            forpix(x,y,src){
                if(x == lu.x || y == lu.y || x == br.x || y == br.y)
                    Pixels(src, x,y) +=  color;
            }
        }

        void inflate(IplImage *src, unsigned char color) {
            IplImage* buffer = cvCloneImage(src);

            forpix(x, y, src) {
                if (Pixels(src, x, y) == color) {

                    forbrd(dx, dy) {
                        Pixels(buffer, x + dx, y + dy) = color;
                    }
                }
            }
            cvCopyImage(buffer, src);
            cvReleaseImage(&buffer);
        }

        void clean(IplImage* src) {
            bool repeat;
            do {

                repeat = false;

                forpix(x, y, src) {

                    int sum = 0;
                    unsigned char must = Pixels(src, x, y) > 128 ? WHITE : BLACK;
                    for (int dx = -1; dx < 2; dx++) {
                        for (int dy = -1; dy < 2; dy++) {

                            if (dx == 0 && dy == 0) {
                                Pixels(src, x + dx, y + dy) = must;
                            }
                            sum += (Pixels(src, x + dx, y + dy) > 128) ? 1 : 0;
                        }
                    }
                    if (sum < 4) {
                        Pixels(src, x, y) = BLACK;
                        repeat = (must == WHITE);
                    }
                }
            } while (repeat);

        }

        void fix(IplImage* src) {
            IplImage* buffer = cvCloneImage(src);

            bool repeat;
            do {

                repeat = false;

                forpix(x, y, src) {

                    int sum = 0;
                    unsigned char must = Pixels(src, x, y) > 128 ? WHITE : BLACK;
                    for (int dx = -1; dx < 2; dx++) {
                        for (int dy = -1; dy < 2; dy++) {

                            if((dx == -1 || dx == 1) && (dy == -1 || dy == 1)){
                                sum += (Pixels(src, x + dx, y + dy) > 128) ? 10 : 0;
                            }else{
                                sum += (Pixels(src, x + dx, y + dy) > 128) ? 1 : 0;
                            }
                            
                        }
                    }
                    if (sum == 12 || sum == 22 || sum == 33) {
                        Pixels(buffer, x, y) = WHITE;
                        repeat = (must == BLACK);
                    }
                }
                cvCopyImage(buffer, src);
            } while (repeat);
            
            cvReleaseImage(&buffer);
        }

        void artefactize(IplImage * src) {

            forpix(x, y, src) {
                if (rand() % 2 == 0) {
                    Pixels(src, x, y) = WHITE;
                }
            }
        }

        bool floodCut(IplImage* src, IplImage* dest, CvPoint anchor, CvPoint &lu, CvPoint &br){
            if(Pixels(src, anchor.x, anchor.y) != WHITE){
                return false;
            }
            lu = anchor;
            br = anchor;
            forpix(x,y,dest){
                Pixels(dest, x, y) = BLACK;
            }
            CvPoint* stack = (CvPoint*)malloc(src->height*src->widthStep*sizeof(CvPoint)*8);
//            cout << (stack == 0 ? "true" : "false") << endl;
            int top = 0;

            
            stack[top] = anchor; top++;
            while(top>0){
                CvPoint curr = stack[--top];
                lu.x = MIN(lu.x, curr.x);
                lu.y = MIN(lu.y, curr.y);
                br.x = MAX(br.x, curr.x);
                br.y = MAX(br.y, curr.y);
                Pixels(src,curr.x,curr.y) = BLACK;
                Pixels(dest,curr.x,curr.y) = WHITE;
                ford(dx,dy){
                    if(Pixels(src,curr.x+dx,curr.y + dy) == WHITE){
                        stack[top].x = curr.x+dx;
                        stack[top].y = curr.y +dy;
                        top++;
                    }
                }
            }

            free(stack);
            return true;
        }


    };
}

#endif

