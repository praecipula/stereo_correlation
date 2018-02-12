#ifndef CAMERA_UNDISTORT_H
#define CAMERA_UNDISTORT_H

#include "imagepipelinestepbase.h"
#include "algorithm/stereo/cameracalibration.h"

namespace Stereo {

    using namespace Algo;
    /**
     * @brief The UndistortCamera step
     * Given a set of distortion parameters, this class will take a source image
     * and undistorit it accourding to those parameters into a destination image.
 */
    class CameraUndistort : public ImagePipelineStepBase
    {

    public:
        CameraUndistort(CameraCalibration::CalibrationResults::ptr calib) :
          ImagePipelineStepBase(),
          m_calib(calib) {}

        virtual ~CameraUndistort() {}

        virtual image_list execute(const image_list& inputs);

    private:
        CameraCalibration::CalibrationResults::ptr m_calib;
    };
}

#endif // CAMERA_UNDISTORT_H
