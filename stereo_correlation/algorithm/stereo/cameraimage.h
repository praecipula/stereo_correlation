#ifndef CAMERAIMAGE
#define CAMERAIMAGE

#include "imagebase.h"

namespace Stereo
{
    namespace Algo
    {
        using namespace cv;

        /**
         * A CameraImage is, semantically, an image that is raw - straight out of a camera.
         * It can be passed ot a StereoHardware::Camera model in order to, for example,
         * undistort it (yielding an UndistortedImage), as long as that Camera is calibrated.
         */
        class CameraImage : public ImageBase
        {

            // Some convenience typedefs
            typedef shared_ptr<CameraImage> ptr;

        };

    }
}

#endif // CAMERAIMAGE

