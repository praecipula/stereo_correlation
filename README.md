# stereo_correlation

This project exists because there are no maintained, modern, cross-platform projects for editing and compositing two side by side images into one stereo image of varying formats. Existing solutions are either overly engineered for a particular task, hard to automate for simple tasks, built for one specific platform and one set of cameras, and lack certain useful features for content creation.

A prototype of this software was constructed in Ruby for proof-of-concept, but after a time I felt it would be worthwhile to port this over to C++ for speed, direct native access to the best tools and libraries, and to rewrite some of the core pipeline to be more flexible and consistent.

The desired feature list is as follows:

* Composable or programmable pipeline
* 2-image correlation of the left and right images
* Red-blue anaglyph generation
* Floating frame generation
* Crop and resize capability
* Lens undistort as a pipeline step
* Automated correlation finding (edge detection and correlation)


Stretch features:
* Generation of 3d scene from correlation data (construct geometry in OpenGL which allows small amounts of rotation and translation of the recreated scene)
