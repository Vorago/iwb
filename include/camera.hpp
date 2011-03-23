/* 
 * File:   camera.hpp
 * Author: fishi
 *
 * Created on Štvrtok, 2011, marec 17, 23:59
 */

#ifndef CAMERA_HPP
#define	CAMERA_HPP

#define NO_CALIBRATION

#include "presentation.hpp"
#include "capture.hpp"

namespace iwb {
    /**
     * Class representing the camera.
     * It contains the camera geomtery and does the camera calibration.
     * @return
     */
    class Camera {
    public:
        static Camera* getInstance();
        ~Camera();
        int getWidth();
        int getHeight();
        int getProjectorHeight();
        int getProjectorWidth();
        CvPoint getProjectorOrigin();
        void calibrate(Capture* cpt, Presentation* prs);
    protected:
        Camera();
    private:
        static Camera* instance;
        int width;
        int height;
        int projectorWidth;
        int projectorHeight;
        CvPoint projectorOrigin;
    };
}

#endif	/* CAMERA_HPP */

