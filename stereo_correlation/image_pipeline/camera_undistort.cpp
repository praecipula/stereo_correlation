#include <opencv2/imgproc.hpp>
#include "camera_undistort.h"
#include "stereoexception.h"

namespace Stereo {

    ImagePipelineStepBase::image_list CameraUndistort::execute(const image_list& inputs)
    {
        //Must have one and only one image.
        STEREO_ASSERT(inputs.size() == 1);
        image_ptr input = inputs.front();
        // Just for debugging purposes
        input->save("/tmp/input.jpg");
        LOGI << m_calib->cameraMatrixK;

        cv::Mat newCameraMatrix;
        cv::Size newSize(6000, 4000);
        cv::Mat firstMap;
        cv::Mat secondMap;

        LOGI << m_calib->distortionCoefficientsD;
        cv::initUndistortRectifyMap(m_calib->cameraMatrixK, m_calib->distortionCoefficientsD,
                                    cv::Mat(), newCameraMatrix, newSize,
                                    CV_32FC1, firstMap, secondMap);
        cv::Mat undistorted;
        LOGI << newSize;
        cv::remap(*input, undistorted, firstMap, secondMap, cv::INTER_LANCZOS4, cv::BORDER_CONSTANT, 0);
        ImageBase::ptr output(new Algo::ImageBase(undistorted));
        stringstream stream;
        stream << "/tmp/undistort.jpg";
        output->save(stream.str());
        image_list outputs;
        outputs.push_back(output);
        return outputs;
    }
}
