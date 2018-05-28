#ifndef IMAGE_PIPELINE_TEST
#define IMAGE_PIPELINE_TEST

#include <list>
#include <QCoreApplication>
#include <opencv2/core.hpp>
#include "image_pipeline/imagepipeline.h"

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

/**
 * @brief RememberNumber - a very small impl of ImagePipelineStepBase
 *
 * This is a good place to see a reference implementation that does the
 * smallest possible compatible thing as its interface
 */
struct RememberNumber : public ImagePipelineStepBase
{
    // Simplifying typedef
    typedef std::shared_ptr<RememberNumber> shared_ptr;
    // Every impl should have these static strings.
    // Version is a number to enable semver with multiple decimals.
    static const std::string s_key;
    static const std::string s_version;

    /* Properties constructor. Nest the properties in a block under the
      key of this class, i.e. this will serialize to:
    "remember_number": {
        "number": "1",
        "version": "1.0"
    }
    */
    RememberNumber(int num) {
        memo data;
        data.put("number", num);
        data.put("version", RememberNumber::s_version);
        m_metadata.add_child(this->key(), data);
    }
    //Memo "copy constructor" passed to super
    RememberNumber(memo meta): ImagePipelineStepBase(meta) {}
    virtual std::string describe() const {
        std::stringstream stream;
        stream << this->key() << ": " << this->number();
        return stream.str();
    }
    // Basically a no-op of the inputs.
    void execute(const ImagePipeline& graph){return;}
    virtual std::string key() const {return RememberNumber::s_key;}

    // Accessors should reach into the metadata memo to get any property that
    // needs to be persisted (local or memoized data is fine to not put in the memo)
    int number() const {return this->data().get<int>("number");}
    // Each class has to implement a load method, but they'll almost always look
    // like this with a subclassed type of shared_ptr
    static ImagePipelineStepBase::shared_ptr load(ImagePipelineStepBase::memo metadata)
    {
        return RememberNumber::shared_ptr(new RememberNumber(metadata));
    }

    ImagePipelineStepBase::memo data() const {return this->m_metadata.get_child(this->key());}
};
// Finally, need to define the constants.
const std::string RememberNumber::s_key = "remember_number";
const std::string RememberNumber::s_version = "1.0";

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
    EXPECT_EQ(std::dynamic_pointer_cast<RememberNumber>(it->lock())->number(), 2);
    it++;
    EXPECT_EQ(std::dynamic_pointer_cast<RememberNumber>(it->lock())->number(), 1);
    it++;
    EXPECT_EQ(std::dynamic_pointer_cast<RememberNumber>(it->lock())->number(), 3);
}

TEST_F(ImagePipelineTest, PipelineCanBeSavedToFile) {
    FriendlyImagePipeline pipe;
    RememberNumber::shared_ptr num_one = RememberNumber::shared_ptr(new RememberNumber(1));
    FriendlyImagePipeline::PipelineStepId h_num_one = pipe.add_node(num_one);
    pipe.save("/tmp/pipetest.3dp");
}

TEST_F(ImagePipelineTest, PipelineCanBeDryRunned) {
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
    // Generate the dry run log
    std::list<std::string> execution_log = pipe.dry_run();
    std::list<std::string>::iterator it = execution_log.begin();
    EXPECT_EQ(*it, "remember_number: 2");
    ++it;
    EXPECT_EQ(*it, "remember_number: 1");
    ++it;
    EXPECT_EQ(*it, "remember_number: 3");
}

#endif // IMAGE_PIPELINE_TEST
