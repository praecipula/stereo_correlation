#ifndef SAVEIMAGE_H
#define SAVEIMAGE_H

#include <string>
#include "imagepipelinestepbase.h"


namespace Stereo
{

    /*
     * SaveImage will, given a filename, save an image to the filesystem.
     */
    class SaveImage: public ImagePipelineStepBase
    {
    public:
      /*
     * A delayed filename constructor, to prompt the user with a dialog
     * in order to save the image.
     */
      SaveImage(const std::string filename);
      SaveImage(const SaveImage& other);
      virtual ~SaveImage() {}

      // Deprecated
      void promptForFilename();

      virtual image_list execute(const image_list& inputs);

    protected:
      std::string m_filename;
    };

}
#endif // SAVEIMAGE_H
