#include "model/stereo_hardware/camera.h"

namespace Stereo
{
    namespace Hardware
    {
        using namespace cv;

        Camera::Camera(const CameraMatrix& cameraMatrix, const DistortionMatrix& distortionMatrix) :
            m_cameraMatrix(cameraMatrix), m_distortionMatrix(distortionMatrix)
        {}

    }
}
