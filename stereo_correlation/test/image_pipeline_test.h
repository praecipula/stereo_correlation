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
    using ImagePipeline::add_dependency;
    using ImagePipeline::add_node;

};

class ImagePipelineTest : public ::testing::Test {
public:

protected:

};

struct RememberNumber : public ImagePipelineStepBase
{
    typedef std::shared_ptr<RememberNumber> shared_ptr;

    RememberNumber(int num) {number = num;}
    image_list execute(const image_list& inputs){return inputs;}
    int number;
};

TEST_F(ImagePipelineTest, PipelineIsZeroLengthConstructable) {
    FriendlyImagePipeline pipe;
    EXPECT_EQ(pipe.size(), 0);
}

TEST_F(ImagePipelineTest, PipelineCanAddDependency) {
    FriendlyImagePipeline pipe;
    RememberNumber::shared_ptr num_one = RememberNumber::shared_ptr(new RememberNumber(1));
    RememberNumber::shared_ptr num_two = RememberNumber::shared_ptr(new RememberNumber(2));
    FriendlyImagePipeline::PipelineStepId h_num_one = pipe.add_node(num_one);
    FriendlyImagePipeline::PipelineStepId h_num_two = pipe.add_node(num_two);
    pipe.add_dependency(h_num_one, h_num_two);
    EXPECT_EQ(pipe.size(), 2);
}

TEST_F(ImagePipelineTest, PipelineCanBeTopologiallySorted) {
    FriendlyImagePipeline pipe;
    RememberNumber::shared_ptr num_one = RememberNumber::shared_ptr(new RememberNumber(1));
    RememberNumber::shared_ptr num_two = RememberNumber::shared_ptr(new RememberNumber(2));
    RememberNumber::shared_ptr num_three = RememberNumber::shared_ptr(new RememberNumber(3));
    FriendlyImagePipeline::PipelineStepId h_num_one = pipe.add_node(num_one);
    FriendlyImagePipeline::PipelineStepId h_num_two = pipe.add_node(num_two);
    FriendlyImagePipeline::PipelineStepId h_num_three = pipe.add_node(num_three);
    // One depends on two
    pipe.add_dependency(h_num_one, h_num_two);
    // Three depends on one
    pipe.add_dependency(h_num_three, h_num_one);
    FriendlyImagePipeline::OrderedSteps steps = pipe.evaluate_processing_order();
    // Therefore, order is 2, 1, 3
    FriendlyImagePipeline::OrderedSteps::iterator it = steps.begin();
    EXPECT_EQ(std::dynamic_pointer_cast<RememberNumber>(it->lock())->number, 2);
    it++;
    EXPECT_EQ(std::dynamic_pointer_cast<RememberNumber>(it->lock())->number, 1);
    it++;
    EXPECT_EQ(std::dynamic_pointer_cast<RememberNumber>(it->lock())->number, 3);
}
#endif // IMAGE_PIPELINE_TEST
