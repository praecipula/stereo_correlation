#ifndef DISPARITY
#define DISPARITY

#include "imagebase.h"
#include <opencv2/core/mat.hpp>

namespace Stereo
{
    namespace Algo
    {
        using namespace cv;

        /**
         * The semantic class for a Disparity image.
         */
        class Disparity : public ImageBase
        {
        public:
            Disparity(Mat cvImage) :
                ImageBase(cvImage)
            {}

            Disparity(size_t rows, size_t cols) :
                ImageBase(rows, cols, DataType<float>::type)
            {}


            /**
             * Load a PFM file.
             * WARNING: this is test-only code at this point. We're likely to only want to use OpenEXR
             * for our real floating point hdr-valued files, this function only exists because Middlebury
             * distributed their ground-truth disparities in pfm format. We make a _bad_ assumption in
             * this function in order to use it for workflow testing, and we should _never_ use this in
             * any context other than testing.
             * @param filename the pfm file to open
             * @return a Disparity image pointer.
             */
            static ImageBase::ptr load_pfm(const string& filename);

        };
    }

}

#endif // DISPARITY

