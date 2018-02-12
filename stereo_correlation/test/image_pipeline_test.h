#ifndef IMAGE_PIPELINE_TEST
#define IMAGE_PIPELINE_TEST

#include "image_pipeline/imagepipeline.h"
#include <QCoreApplication>
#include <opencv2/core.hpp>

using namespace Stereo;
using namespace std;
using namespace cv;


class FriendlyImagePipeline : public ImagePipeline
{
public:

};

class ImagePipelineTest : public ::testing::Test {
public:

protected:

};

TEST_F(ImagePipelineTest, PipelineIsZeroLengthConstructable) {
    FriendlyImagePipeline pipe;
    EXPECT_EQ(pipe.size(), 0);
}
#endif // IMAGE_PIPELINE_TEST
