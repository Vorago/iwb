/* 
 * File:   analysis.cpp
 * Author: vorago
 * 
 * Created on December 23, 2010, 1:10 PM
 */

#include "include/analysis.hpp"
namespace iwb {
IplImage*  analysis::getDiff(IplImage *frame1, IplImage *frame2) {
    IplImage* diff;
    diff = cvCreateImage( cvGetSize(frame1), frame1->depth, frame1->nChannels );
    int height = frame1->height;
    int width = frame1->width;
    for (int i=0; i<height; i++)
    {
        for (int j=0; j<width; j++)
        {
            CvScalar fr1;
            CvScalar fr2;
            fr1=cvGet2D(frame1,i,j);
            fr2=cvGet2D(frame2,i,j);

            if(fr1.val[0]!=fr2.val[0]&&fr1.val[1]!=fr2.val[1]&&fr1.val[2]!=fr2.val[2])
            {
                fr2.val[0]=0;
                fr2.val[1]=0;
                fr2.val[2]=0;
                cvSet2D(diff,i,j,fr2);
            }
            else
            {
                fr1.val[0]=255;
                fr1.val[1]=255;
                fr1.val[2]=255;
                cvSet2D(diff,i,j,fr1);
            }
        }
    }
    return diff;
}
}