#include "imagepipelinestepbase.h"

namespace Stereo
{
    ImagePipelineStepBase::ImagePipelineStepBase(std::string subclass_key, std::string subclass_version)
    {
        memo data;
        data.put("version", subclass_version);
        m_metadata.add_child(subclass_key, data);
    }

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
