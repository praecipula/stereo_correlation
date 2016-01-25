#include "imagebase.h"

namespace Stereo
{
    namespace Algo
    {

        static ImageBase::ptr ImageBase::load(const string& filename)
        {
            return ImageBase::ptr(new ImageBase(imgread(filename)));
        }
    }
}

