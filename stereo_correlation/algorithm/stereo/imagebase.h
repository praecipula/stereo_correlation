#ifndef IMAGEBASE
#define IMAGEBASE

#include <opencv2/core/mat.hpp>
#include <memory>
#include <string>

namespace Stereo
{
    namespace Algo
    {
        using namespace cv;
        using namespace std;

        /**
         * A ImageBase is the base class for all images that can be used in e.g. OpenCV contexts.
         *
         * The intent is that this image can be used in other contexts as well - e.g. to be displayed by OpenGL.
         * This class is the mediator class that handles the communication across all such interfaces.
         *
         * Implementation-wise, it inherits from Mat of OpenCV and goes from there.
         */
        class ImageBase : public Mat
        {
        public:

            typedef std::shared_ptr<ImageBase> ptr;
            typedef std::shared_ptr<const ImageBase> const_ptr;

            static ImageBase::ptr load(const string& filename);

            /**
             * Construct from OpenCV mat
             * @param imageMatrix the OpenCV matrix
             */
            ImageBase(const Mat& imageMatrix) :
                Mat(imageMatrix)
            {}

            ImageBase(size_t width, size_t height, int type) :
                Mat(width, height, type)
            {}

            /** Get a grayscale copy of this image.
             */
            ptr grayscale_copy();

        };

    }
}


#endif // IMAGEBASE

