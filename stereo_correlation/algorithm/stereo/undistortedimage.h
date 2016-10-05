#ifndef UNDISTORTEDIMAGE
#define UNDISTORTEDIMAGE

#include "imagebase.h"

namespace Stereo
{
    namespace Algo
    {
        using namespace cv;

        /**
         * An UndistortedImage is, semantically, an image that is camera-independent
         * It has been rectified from a raw image into a rectilinear-projection image,
         * which is what is expected for most stereo algorithms.
         */
        class UndistortedImage : public ImageBase
        {

        };

    }
}


#endif // UNDISTORTEDIMAGE

