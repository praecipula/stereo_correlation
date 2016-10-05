#include "imagebase.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
namespace Stereo
{
    namespace Algo
    {

        ImageBase::ptr ImageBase::load(const string& filename)
        {
            return make_shared<ImageBase>(imread(filename));
        }

        ImageBase::ptr ImageBase::grayscale_copy()
        {
            Mat viewGray;
            //TODO: Assumption about source colorspace here. We should track that in this object.
            cvtColor(*this, viewGray, CV_BGR2GRAY);
            return make_shared<ImageBase>(viewGray);
        }
    }
}

