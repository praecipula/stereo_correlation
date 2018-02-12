#include <iostream>
#include <list>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
// Someday I'd like to get rid of all Qt dependencies for our CLI version.
#include <QStandardPaths>

#include "common.h"
#include "stereoexception.h"
#include "strcon.h"
#include "appresources.h"
#include "algorithm/stereo/cameracalibration.h"
#include "algorithm/stereo/imagebase.h"
#include "image_pipeline/openimage.h"
#include "image_pipeline/camera_undistort.h"
#include "image_pipeline/saveimage.h"


using namespace std;
namespace S = Stereo;
namespace Alg = Stereo::Algo;

int main(int argc, char* argv[])
{
    //TODO: QT_CLI
    QCoreApplication a(argc, argv);

    S::initialize();
    list<string> files;
    for (int i = 0; i < argc; ++i)
    {
        files.push_back(string(argv[i]));
    }
    // We basically don't care about this (executable name) for now, so unshift it.
    files.pop_front();
    // Load our calibration
    Alg::CameraCalibration::CalibrationResults::ptr calibResults(new Alg::CameraCalibration::CalibrationResults());
    QFileInfo calibrationFileInfo(S::StrCon(Stereo::AppResources::cameraCalibrationPath("gopro_4_12mp_wide")).to_qs());
    cout << S::AppResources::cameraCalibrationPath("gopro_4_12mp_wide");
    if (!calibrationFileInfo.exists())
    {
        std::stringstream message;
        message << "Calibration file at " << S::StrCon(calibrationFileInfo.canonicalFilePath()).to_s() << " does not exist";
        STEREO_LOG_ASSERT(false, message.str());
    }
    string calibrationFile = S::StrCon(calibrationFileInfo.canonicalFilePath()).to_s();
    calibResults->deserialize(calibrationFile);
    for (list<string>::iterator it = files.begin();
         it != files.end();
         ++it)
    {
        /*
         * Open image
         * Undistort image
         * Set correlation
         * Set image crop (either before or after)
         * Option a: execute crop
         *   Borderify image
         *   Profit - this is pre-border crop.
         *
         * Option b: set image border (after crop above)
         *   Backfit crop (so the effective result is the desired crop ratio
         *   Apply border
         *
         *
         * Note: Discussing with Laurie, we think that doing something like a tryptich would be best served with a
         * flexible pipeline, which argues for duplicate displacement. This can be done feed-forward, where the
         * pipeline is hand-tuned for this use case, and crop can happen before border.
         *
         * That being said, there is a photo gallery like use case I would like to support where the crop ratio takes
         * the displacement into account and pre-crops the images such that the final crop is consistent - this makes
         * seeing tiled images more natural. The effect of variable image crop is not too distracting after the images
         * are processed, so in situations where one would view a lot of images, post-border-and-displacement-crop is
         * a fine way to go. This means that there would be multiple crop steps - setting the crop, then using the
         * displacement + border dimensions to backfit the image crop at the previous step :/
         *
         * So, I think that the digest is this:
         *
         * The pre-step of crop needs to happen at a user-specified step
         * The dependencies should be explicit
         * Crop would be nice to tie into any post-modify node step (so an intervening step could be
         * a resize, and the crop could move through correctly)
         *
         * So there is *path* dependence from crop TO the output - it can't just crop based on the output,
         * it might need to crop with a scale in mind. Ehh.
         *
         * One solution could be to pass a memo of relevant operations down the chain, and all operations that,
         * say, modify width could pass their multipliers / subtractors etc... down the chain. This can then
         * be solved later for the correct answer upstream
         *
         * This feels like a lot of work.
         *
         * So let's make the human do that for now: all images are pre-crop and we just log the operations so that
         * it's clear how the changes might need to be applied in the correct processing situations if they
         * come up.
         *
         * =====
         *
         * Pretty sure the Strategy method with maybe a memo is the way to go.
         *
         * Examples of some workflows:
         * (file) Open image (L1)
         * (file) Open image (R1)
         * (L1) Undistort (L2)
         * (R1) Undistort (R2)
         * (L2) Border with floating frame (L3 + new coord_frame)
         * (R2) Border with floating frame (R3 + new coord_frame)
         * (L3) Color simple anaglyph left (L4)
         *     or: (L3, R3) Color dubois anaglyph left (L4) - 2 to 1
         * (R3) Color simple anaglyph right (R4)
         * (coord_frame) Correlate pointing errors (pterr)
         * (coord_frame) Select correspondence point (corr)
         *
         * OK, structure is this:
         * Layers that have a position and are drawn back to front
         * The geometry / geography of the layer is based on an x, y, w, h frame
         * Output of layer can be one or multiple layers (think dubois - 2 in, 1 out)
         * Input: one or more lower layers (can be other layers - what will UI look like?)
         * (This makes not a tree, but a dag)
         */
        S::OpenImage opener(*it);
        S::ImagePipelineStepBase::image_list source = opener.execute(Stereo::ImagePipelineStepBase::image_list());
        S::CameraUndistort undistort(calibResults);
        S::ImagePipelineStepBase::image_list undistorted = undistort.execute(source);
        S::SaveImage saver("/tmp/undistort.jpg");
        saver.execute(undistorted);

    }


}
