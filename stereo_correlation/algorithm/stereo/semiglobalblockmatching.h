#ifndef SEMIGLOBALBLOCKMATCHING_H
#define SEMIGLOBALBLOCKMATCHING_H

#include "disparity.h"

namespace Stereo
{
    namespace Algo
    {
        using cv;

        /**
         * Calculate a disparity map using semi-global block matching from OpenCV.
         * This is a fairly thin veneer over cv::StereoSGBM in order to allow for
         * default parameters and parameter access and serialization. A common use
         * case may be to try to "dial in" the matching
         */
        class SemiGlobalBlockMatching
        {
            static const int s_assumedChannelDepth = 8;
            static const int s_assumedWindowSize = 5;
            /**
             * Constructor. The following comes straight from cv::StereoSGBM.
             * @param minDisparty Minimum possible disparity value. Normally, it is zero but sometimes rectification
             * algorithms can shift images, so this parameter needs to be adjusted accordingly.
             * @param numDisparities Maximum disparity minus minimum disparity. The value is always greater than zero.
             * In the current implementation, this parameter must be divisible by 16
             * @param windowSize Matched block size. It must be an odd number >=1 . Normally, it should be somewhere in the 3..11 range.
             * @param singleDisparitySmoothness The first parameter controlling the disparity smoothness. See below.
             * @param multipleDisparitySmoothness The second parameter controlling the disparity smoothness. The larger the values are,
             * the smoother the disparity is. P1 is the penalty on the disparity change by plus or minus 1 between neighbor pixels.
             * P2 is the penalty on the disparity change by more than 1 between neighbor pixels.
             * The algorithm requires P2 > P1.
             * @param maxDisparityDifference Maximum allowed difference (in integer pixel units) in the left-right disparity check.
             * Set it to a non-positive value to disable the check.
             * @param derivativeCap Truncation value for the prefiltered image pixels. The algorithm first computes x-derivative at each
             * pixel and clips its value by [-preFilterCap, preFilterCap] interval.
             * The result values are passed to the Birchfield-Tomasi pixel cost function
             * @param costVictoryPercent Margin in percentage by which the best (minimum) computed cost function value should “win”
             * the second best value to consider the found match correct. Normally, a value within the 5-15 range is good enough
             * @param speckleWindow Maximum size of smooth disparity regions to consider their noise speckles and invalidate.
             * Set it to 0 to disable speckle filtering. Otherwise, set it somewhere in the 50-200 range.
             * @param speckleRange Maximum disparity variation within each connected component. If you do speckle filtering,
             * set the parameter to a positive value, it will be implicitly multiplied by 16. Normally, 1 or 2 is good enough.
             * @param runTwoPass Set it to true to run the full-scale two-pass dynamic programming algorithm. It will consume
             * O(W*H*numDisparities) bytes, which is large for 640x480 stereo and huge for HD-size pictures. By default, it is set to false .
             */
            SemiGlobalBlockMatching(int minDisparity = 5,                       // to account for image shift via rectification
                                    int numDisparities = 16,                    // Eh. A dozen different depths.
                                    int windowSize = s_assumedWindowSize,       // the "semi-global" window to consider
                                    int singleDisparitySmoothness =             // Smoothness param penalizing changes of +- 1 level
                    8 * s_assumedChannelDepth * s_assumedWindowSize * s_assumedWindowSize,
                                    int multipleDisparitySmoothness =           // Smoothness param penalizing changes of +- N>1 levels
                    32 * s_assumedChannelDepth * s_assumedWindowSize * s_assumedWindowSize,
                                    int maxDisparityDifference = 0,             // Max allowed disparity delta (left to right). "Fuzzy" compare of disparity neighborhood
                                    int derivativeCap = 0,                      // Pre-filter which clamps x-derivative to a certain delta
                                    int costVictoryPercent = 5,                 // Margin by percent that "winning" cost function needs to win by to be selected
                                    int speckleWindow = 0,                      // Window to despeckle over - 0 diable, otherwise 50-200 recommended
                                    int speckleRange = 0,                       // Range at which a variance is considered a speckle. Low integer.
                                    int runTwoPass = 0                          // Run a full 2-pass algorithm. Very memory intensive.
                                    );

            /**
              * Calculate and return the disparity map.
              * @param left – Left 8-bit single-channel or 3-channel image.
              * @param right – Right image of the same size and the same type as the left one.
              * @return disp – Output disparity map. It is a 16-bit signed single-channel image of the same size as the input image.
              * It contains disparity values scaled by 16. So, to get the floating-point disparity map, you need to divide
              * each disp element by 16.
              */
            Dispartiy operator() (const Mat& left, const Mat& right);

            /* TODO: depending on the run time of the algorithm, we may want to live-regenerate the disparity map when any params change.
             * Alternatively, I can see that the _entire workflow_ can be parallelized and rerun when any of the workflow params creates
             * a "regenerable" change, so that's just as likely to be where things happen.
             */

        };
    }
}

#endif
