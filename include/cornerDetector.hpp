#ifndef CORNERDETECTOR_HPP
#define	CORNERDETECTOR_HPP

#include <iostream>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#define WHITE 255
#define BLACK 0
#define forpix(x,y,src)   for(int y=0; y<src->height; y++) for(int x=0; x<src->width; x++)
#define forpixc(x,y,src,lu,br)   for(int y=lu.y; y<=br.y; y++) for(int x=ul.x; x<=br.x; x++)
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
//            inflate(src, WHITE);
//            inflate(src, BLACK);
            
            IplImage* fragment = cvCloneImage(src);
            forpix(x,y,src){
                CvPoint point;
                point.x = x;
                point.y = y;
                CvPoint lu, br;
                if(floodCut(src, fragment, point, lu, br)) {
                    fix(fragment, lu, br);
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

        bool sameVertical(IplImage* src, int x, int dx, int y1, int y2){
            for(int y = y1; y<=y2; y++){
                if(Pixels(src,x,y) != Pixels(src,x+dx,y)) return false;
            }
            return true;
        }

        bool sameHorizontal(IplImage* src, int y, int dy, int x1, int x2){
            for(int x = x1; x<=x2; x++){
                if(Pixels(src,x,y) != Pixels(src,x,y+dy)) return false;
            }
            return true;
        }

        bool isACorner(IplImage* src, CvPoint ul, CvPoint br, bool isUpperLeft){

            int x1=ul.x+1;
            int x2=br.x-1;
            int y1=ul.y+1;
            int y2=br.y-1;

            int i;
            int x;
            bool areTheSame;

            //------------------------------------------------------------------------------------

            while(sameVertical(src, x1, -1 , ul.y, br.y)) x1++;
            while(sameVertical(src, x2, 1 , ul.y, br.y)) x2--;
            while(sameHorizontal(src, y1, -1, ul.x, br.x)) y1++;
            while(sameHorizontal(src, y2, 1, ul.x, br.x)) y2--;
            x1--; x2++; y1--; y2++;
            //------------------------------------------------------------------------------------

//            cout << "-- RESULT" << endl;
//            cout << x1  << " : " << y1 << endl;
//            cout << x2  << " : " << y2 << endl;
//            dump(src, cvPoint(x1,y1), cvPoint(x2,y2));
            
//            if(isUpperLeft){
//                //check for UL corner
//            }else{
//                //check for BR corner
//            }
            if(((x2-x1 != 1) || (y2-y1 != 1))) return false;

            int sum = 0;
            for(int i=y1; i<=y2;i++){
                for(int j=x1; j<=x2; j++){
                    if(Pixels(src,j,i) == WHITE) sum++;
                }
            }

            return (sum == 3);
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

        void fix(IplImage* src, CvPoint ul, CvPoint br) {
            IplImage* buffer = cvCloneImage(src);

            bool repeat;
            do {

                repeat = false;

                forpixc(x, y, src, ul, br) {

                    int sum = 0;
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
                        repeat = true;
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

