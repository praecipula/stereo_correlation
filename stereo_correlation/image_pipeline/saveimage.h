#ifndef SAVEIMAGE_H
#define SAVEIMAGE_H

#include <string>
#include "stereoexception.h"
#include "imagepipelinestepbase.h"
#include "pipelinestepacceptingimage.h"


namespace Stereo
{

    /*
     * SaveImage will, given a filename, save an image to the filesystem.
     */
    class SaveImage: public ImagePipelineStepBase, public PipelineStepAcceptingImage
    {
    public:
        typedef std::shared_ptr<SaveImage> ptr;

        static const std::string s_key;
        static const std::string s_version;

        SaveImage();
        SaveImage(const SaveImage& other);
        SaveImage(const std::string& imageFilename);
        SaveImage(const memo& metadata);
        virtual ~SaveImage() {}

        // Deprecated
        void promptForFilename();

        std::string filename() const;

        // Return a const copy of the metadata under the key for quick property inspection.
        ImagePipelineStepBase::memo data() const {return this->m_metadata.get_child(this->key());}

        virtual std::string describe() const;
        virtual void execute(const ImagePipeline& pipeline);
        virtual std::string key() const {return SaveImage::s_key;}
        virtual std::string version() const {return SaveImage::s_version;}
        static ImagePipelineStepBase::shared_ptr load(memo metadata);

    protected:
        ImagePipelineStepBase::memo& mutable_data() {return this->m_metadata.get_child(this->key());}
    };

    class SaveImageNoFilenameException : public AssertionFailure
    {};
}
#endif // SAVEIMAGE_H
