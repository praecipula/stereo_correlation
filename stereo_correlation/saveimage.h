#ifndef SAVEIMAGE_H
#define SAVEIMAGE_H

#include <string>
#include "imagepipelinestepbase.h"

/* SaveImage is constructed with a filename, and assumes
 * a single input image in its execute() step.
 */
class SaveImage: public ImagePipelineStepBase
{
public:
    /*
     * A delayed filename constructor, to prompt the user with a dialog
     * in order to save the image.
     */
    SaveImage();
    /*
     * Programmatic constructor for use where we know the filename ahead of time.
     */
    SaveImage(const std::string& filename);

    virtual ~SaveImage(){}

    void promptForFilename();

    virtual image_list execute(const image_list& inputs);

protected:
    std::string m_filename;
};

#endif // SAVEIMAGE_H
