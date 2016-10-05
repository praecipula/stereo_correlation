#include "disparity.h"
#include <Magick++.h>

#include <QFile>
#include <QIODevice>
#include <QTextStream>

namespace Stereo
{
    namespace Algo
    {

        ImageBase::ptr Disparity::load_pfm(const string& filename)
        {
            Magick::Image image(filename);
            image.write("/tmp/disp.png");
            // Convert the imagemagick image data to a cv::Mat.
            /* There is likely a better or more efficient way to do this, but I can't quickly find it.
             * ImageMagick has a "pixel cache" that is responsible for abstracting away the method of
             * accessing individual pixels - this is what is used to get to the raw underlying data.
             * The best way I can find to translate from ImageMagick's pixels to our cv::Mat is to
             * iterate over all the pixels and store the values in the mat.
             *
             * It would be better to be able to do an in-memory mapping or cast of some sort, but
             * I'm not sure that the memory layout is identical - and I'll sacrifice runtime for
             * potential bugs in the future.
             */
            Magick::Geometry geo = image.size();
            size_t width = geo.width();
            size_t height = geo.height();
            Magick::Image::ImageStatistics stats;
            image.statistics(&stats);
            /* Sigh. It seems that ImageMagick will scale the float values in the pfm file to fit
             * within its quantum value (16 bit unsigned as of this testing) in order to not have
             * to support float pixel values. I can understand that, but for subpixel accuracy
             * matrices, we lose precision here since the values are clamped to a scaled integer
             * of the original float values.
             *
             * Still, it saves a lot of work getting the file read. Therefore, I "unscale" the values
             * by assuming that the minimum disparity represents a disparity of 1.
             *
             * THIS IS WRONG.
             *
             * The "ground truth" disparity values that are stored in the pfm values have sub-pixel
             * accuracy. To say their minimum value is 1 is to ignore the fact that the real values
             * will have measurement precision less than 1.
             *
             * But we can't do any better with ImageMagick.
             *
             * To do this right, we really do need to compile libpfm and use it to read the file.
             * Not doing this for now, as I've pretty much never seen a less-documented library
             * in my life. Why Middlebury chose this, instead of a better-supported library,
             * say, EXR, I'll never know. This will work to test the workflow, however.
             */
            float scaleFactor = static_cast<float>(stats.red.minimum);
            ImageBase::ptr disparity(new Disparity(width, height));
            // Set the size of the disparity matrix
            MatIterator_<float> it = disparity->begin<float>();
            const Magick::PixelPacket* scanner = image.getConstPixels(0, 0, width, height);
            for (size_t j = 0; j < height; ++j)
            {
                for (size_t i = 0; i < width; ++i)
                {
                    /* The value of the matrix is the value at the [i, j] location
                     * divided by the prescaling that ImageMagick did.
                     * We're assuming here that the pixel values are all the same in
                     * R, G, and B, so we can just pluck the red channel.
                     */
                    float value = scanner[j * width + i].red / scaleFactor;
                    *it = value;

                }
            }
            disparity;
            return disparity;
        }
    }
}

