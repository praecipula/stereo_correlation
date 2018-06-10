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
    class SaveImage: public ImagePipelineStepWithCommonImpl<SaveImage>,
            public PipelineStepAcceptingImage
    {
    public:
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

        virtual std::string describe() const;
        virtual void execute(const ImagePipeline& pipeline);
    protected:
    };

    class SaveImageNoFilenameException : public AssertionFailure
    {};
}
#endif // SAVEIMAGE_H
