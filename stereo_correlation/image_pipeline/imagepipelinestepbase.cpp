#include "imagepipelinestepbase.h"

namespace Stereo
{
    ImagePipelineStepBase::ImagePipelineStepBase()
    {}

    ImagePipelineStepBase::ImagePipelineStepBase(const memo &metadata):
        m_metadata(metadata)
    {}

    void ImagePipelineStepBase::append_for_save(memo& parent)
    {
        for(memo::iterator it = this->m_metadata.begin();
            it != this->m_metadata.end();
            ++it)
        {
            parent.put_child(it->first, it->second);
        }
    }
}
