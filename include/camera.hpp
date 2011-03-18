/* 
 * File:   camera.hpp
 * Author: fishi
 *
 * Created on Štvrtok, 2011, marec 17, 23:59
 */

#ifndef CAMERA_HPP
#define	CAMERA_HPP

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
        float getLeftOffset();
        float getRightOffset();
        float getTopOffset();
        float getBottomOffset();
        void calibrate(Capture* cpt, Presentation* prs);
    protected:
        Camera();
    private:
        static Camera* instance;
        int width;
        int height;
        float leftOffset;
        float rightOffset;
        float topOffset;
        float bottomOffset;
    };
}

#endif	/* CAMERA_HPP */

