#include "imagebase.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace Stereo
{
    namespace Algo
    {

        ImageBase::ptr ImageBase::load(const string& filename)
        {
            return make_shared<ImageBase>(imread(filename));
        }

        ImageBase::ptr ImageBase::loadGrayscale(const string& filename)
        {
            return make_shared<ImageBase>(imread(filename, IMREAD_GRAYSCALE));
        }

        ImageBase::ptr ImageBase::grayscale_copy()
        {
            Mat viewGray;
            //TODO: Assumption about source colorspace here. We should track that in this object.
            cv::cvtColor(*this, viewGray, CV_BGR2GRAY);
            return make_shared<ImageBase>(viewGray);
        }

        ImageBase::ptr ImageBase::rect_scaled_copy(double factor)
        {
            Mat viewScaled;
            Size size;
            cv::resize(*this, viewScaled, size, factor, factor, INTER_LANCZOS4);
            return make_shared<ImageBase>(viewScaled);
        }

        void ImageBase::save(const string& filename) {
            imwrite(filename, *this);
        }
    }
}

