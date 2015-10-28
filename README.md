# Stereo correlation

This project exists because there are no maintained, modern, cross-platform projects for editing and compositing two side by side images into one stereo image of varying formats. Existing solutions are either overly engineered for a particular task, hard to automate for simple tasks, built for one specific platform and one set of cameras, overly simplistic, or lack certain useful features for content creation.

Theory:
------

The main method of constructing a 3d image is to recreate our vision system's ability to view a scene with a slight displacement between the eyes. By taking a picture with 2 cameras with a slight offset to each other (or, equivalently, if the scene is static, taking 2 pictures, one after the other, with the camera shifted horizontally slightly - the "cha-cha" method), it is possible to provide the 2 images necessary for our eyes to reconstruct the 3d scene.

(Note: you need a set of anaglyph glasses to view the following examples)

![Anaglyph glasses](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a0/3d_glasses_red_cyan.svg/1024px-3d_glasses_red_cyan.svg.png)

![Waimea Falls](https://raw.github.com/praecipula/stereo_correlation/master/example_images/anaglyph_IMG_9098_IMG_8407.png)

**Waimea Falls, Haleiwa, HI**

Motivation:
------

A prototype using Ruby, Qt, ImageMagick, and Cairo was built to serve as a tool to reconstruct anaglyphs from side-by-side point-and-shoot snapshots. One of the original purposes of the Ruby library - to create a "layers and canvas with DSL operations" - was achieved, but at the cost of inconvenience of using toolkits in an awkward way. For example, calling ImageMagick with command-line exec statements requires the generation of a command-line statement composing library and is not cross-platform; using RMagick to interface with libmagic++ means we have to deal with resource management with C++ idioms in Ruby - and, for example, there is no guaranteed destructor to tie Ruby object lifetime with freeing memory, a very common C++ idiom.

This project deserved a native implementation. I decided to port over to C++, my other most proficient language, for direct access to the tools and libraries used to make this project work. Features at time of completion will be:


* 2-image correlation of the left and right images
* Basic rotation and skew transforms
* Composable or programmable pipeline
* Red-blue anaglyph generation
* Wigglegram generation
* JPS file generation
* Crop and resize capability as a pipeline step
* Floating frame generation as a pipeline step
* Annotation and signature as a pipeline step
* Lens undistort as a pipeline step
* Automated correlation finding (edge detection and correlation)


Stretch features:
* Batch processing of frames from side-by-side movies for generation of 3d movies.
* Generation of 3d scene from correlation data (construct geometry in OpenGL which allows small amounts of rotation and translation of the recreated scene)

A deeper look into the features
------

* 2-image correlation of the left and right images

Given a view which displays the left and the right image as taken by the camera, it is possible to define the image plane by the selection of how the images are displaced relative to each other. The point on the image where both features overlap precisely - that is, the pixel coordinate at which the exact same spot lies in the real-world geometry; the features are perfectly superimposed from both images - this point is the apparent location of the image plane. It's the "screen-level" depth of the image, which allows the compositor to choose whether items in the scene pop out of the screen or descend behind it. Using a mouse-based selection of this point on each image, we can calculate the operations needed to properly align and compose the images on top of one another.

Status in prototype: 100% (fully working)

Status in native: 80% (the view exists, but no operations are yet done to correlate the images together).

* Basic rotation and skew transforms

Constructing 2 lenses side-by-side, whether on one housing or with twinned cameras, has a problem: it's very difficult to build a rig that has precise tolerances which will result in perfect image alignment. Slight manufacturing or mounting issues can result in an image which has one camera slighty rotated relative to the other; additionally, except where very highly toleranced lenses are used, the images may inherently have different pincushion/barrel and skew distortions. A set of basic transforms to align other points in the image (other than the point of overlap) can correct for these real-world phenomenon.

Status in prototype: 0% (relied on strong camera rig; slight rotations of 1-2 degrees done by hand when necessary)

Status in native: 0%

* Composable or programmable pipeline

It's possible to have different workflows for images: for instance, we may want to have a floating frame to correct for edge violations for a certain image, or we may not, or we may want to undistort lens parameters at a certain step in the processing pipeline (i.e. before or after alignment). This feature revolves around using the [command pattern](https://en.wikipedia.org/wiki/Command_pattern) in order to allow a user-selectable pipeline of operations that chain to form the complete workflow.

Status in prototype: 0% (not prototyped, the workflow is reprogrammed for each case)

Status in native: 10% (a single command has been built and hack-tested to prove the concept and establish the api; ui space created for this purpose)

* Red-blue anaglyph generation

Anaglyph glasses are those stereotypical 3d "color filter" glasses with a red lens on the left and blue lens on the right. We can isolate the red chanel on the right image and the cyan (green and blue) color on the left image by multiplying with pure red (#FF000) and pure cyan (#00FFFF). We can then use the compositing operator "screen" to combine the red chanel into the blue image (or vice versa, screen is commutative), which has the effect of pulling the appropriate channels in while lightening the result. This generates an anaglyph image, which is the preferred method of generating static (as opposed to, e.g., a wigglegram) and device-independent (as opposed to requiring special equipment) 3d images, despite the drawback of not retaining true color and having issues with near-red and near-cyan elements in the image.

![Koko Head](https://raw.github.com/praecipula/stereo_correlation/master/example_images/anaglyph_IMG_9222_IMG_8530.png)

**Koko Head, HI. Notice my friend's teal shirt violently "vibrates" as an anaglyph; the right eye sees it as light blue, the left eye sees black.**

Status in prototype: 100%

Status in native: 0%

* Wigglegram generation

A wigglegram is an animated gif of the two images oscillating back and forth. [Here](https://www.flickr.com/photos/rosendahl/4720261964/sizes/o/) is an example (photo credit Flickr user rosendahl). This gives some sense of depth to the image, and preserves the original coloration. It can be displayed on a computer or tablet that does not have specialized hardware for displaying 3d images, although it is not a true stereo image, but rather a parallax-based image.

Status in prototype: 0%

Status in native: 0%

* JPS file generation

A JPS file is a file type for stereoscopic JPEG images. It is essentially a side-by-side construction of 2 JPEG images, with some metadata markers that describe the packing of the images. 3d-aware viewing devices (like my 3d television) can often show these images natively, providing a convenient and portable container for trading images. Some software can construct an anaglyph on the fly to display JPS files on a non-3d-enabled viewing platform.

Status in prototype: 0%

Status in native: 0%

* Crop and resize capability as a pipeline step

These are basic functions that are useful for composition of an image outside of the camera. When viewing 3d images in a gallery, especially when 3d techniques such as a floating frame are used, the images tend to jump around from size to size, which is distracting. To correctly resize the images to appear of the same size requires a resize and crop functionality that operates on the images themselves early in the processing chain, before overlapping and floating frame operations have been applied, as these later steps provide extra padding horizontally, but not vertically, which alter the ability of other tools to successfully resize the images.

Status in prototype: 40% (basic crop and resize work, but are limited in where and how they can be applied).

Status in native: 0%

* Floating frame generation as a pipeline step

A floating frame is a technique to compensate for edge violation. When an image extends in front of the view plane (popping out at the viewer), if a feature crosses the left and right side of the frame, it is immediately obvious and uncomfortable to the viewer. One eye still sees much of the object, where another eye gets blocked by the edge of the frame. This effect does not happen on the top and bottom of the image, because our eyes are mounted side-by-side, leading to displacements that only violate at the left and right edge of the frame. By floating a frame in front of the view plane, we can cut the images not at the view plane (where the true edge of the image is), but at the frame. The effect is to push a frame out to the viewer at the same distance as the frontmost geometry, which corrects the edge violation.

![Hanauma Bay](https://raw.github.com/praecipula/stereo_correlation/master/example_images/anaglyph_IMG_9026_IMG_8334.png)

**Hanauma Bay. Notice the _strongly_ floated frame to correct for edge violation of rock wall.**

Status in prototype: 100%

Status in native: 0%

* Annotation and signature as a pipeline step

When we have a frame, we might as well use it to title and annotate the image with text, as well as apply an artist's signature. To do this correctly, we have to float the annotation with the frame, meaning we have to apply the annotation separately to each frame before the final displacement happens. This pushes the text out of plane at the same time the frame pushes out of frame

Status in prototype: 80% (was working, but a regression caused it to break)

Status in native: 0%

* Lens undistort as a pipeline step

My 3d camera setup has changed; originally, it was 2 point and shoot cameras that had fairly rectilinear lenses with only slight barrel distortion, which was acceptable to leave in the image. The downside to this setup was the interocular distance of the lenses was 64mm, which is right about where the interpupillary distance of the average American man sits (64.7). Since the cameras cannot "toe in" in the same way that human eyes can, this means that there was a strict distance to the nearest object in the scene (or more correctly, a strict distance _ratio_ between the nearest and furthest object). To display correctly, everything in the scene must be seen without any eye crossing; the viewer's eyes have to replicate the near-parallel camera setup, which means the near point of the scene was around 1.5-2m for correct capture.

I recently upgraded to 2 GoPros in order to decrease the interocular distance, to around 40mm. This allows closer portraits to be taken, but has the obvious downside of a highly distorted image. There are good undistort tools available in ImageMagick, but I determined experimentally that the undistort operation must occur at a specific point in the pipeline in order to produce the best results: after image correlation and overlapping. Undistortion prior to correlation can cause excess distortion toward the edges of the image. This is one of the drivers for an explicit image pipeline to exist.

Status in prototype: 0% (experimentation was done with general purpose image manipulation tools)

Status in native: 0%

* Automated correlation finding (edge detection and correlation)

Having a correlation point that ties the images together and defines the view plane is one of the many artistic choices that can be made when compositing the images, but the process of finding such a point can be tedious. Pixel-level precision requires zooming into the image to accurately place the correlation point; but detail is lost to the eye at close approach. A method to assist in selecting correlation points would be of great benefit to the workflow.

This is not a trivial problem, but as a college student, I took a graduate level class which enabled me to work on it and construct an algorithm. The basic idea is to find interesting potential correlation points, then look in the other image for similar color curves in a similar coordinate neighborhood around the selected point. These curves must assume discontinuity at the near vicinity of the point, since the geometry in the foreground is displaced against the background. Matches which hit above a user-supplied confidence level can be treated as potential alignment points.

Status in prototype: 0% (although an algorithm is known)

Status in native: 0%

* Batch processing of frames from side-by-side movies for generation of 3d movies.

All of the above features, when applied to a movie, with additional time-based tweening of some of the parameters (i.e. I've always been interested to determine if a dolly-zoom like effect can be built into a movie by tweening the view plane, which will push or pull the entire scene to the viewer).

* Generation of 3d scene from correlation data (construct geometry in OpenGL which allows small amounts of rotation and translation of the recreated scene)

Given the autocorrelation algorithm described above, if enough points of correlation can be found, a 3d scene can be recreated from the 2d images. The direction of horizontal shift from the view plane point (left or right) determines a point's forward or backward location in the 3d scene; the degree of horizontal shift (displacement in pixels) determines the degree of displacement from the image plane. These data can be used to create a mesh that can be textured with the images in order to recreate a 3d scene that can be slightly panned and tilted to recreate novel views of the original scene, within the limits of the displacement of the cameras to capture. This can be used to create an "advanced wigglegram", for instance, where the image can be smoothly animated between the left and right images, or displayed easily in hardware capable of displaying 3d scenes in stereo (e.g. Oculus Rift). This can also be used to shrink the interpupilary distance between images, or to simulate toe-in and parallel views of the eye, by rendering the scene with different camera parameters than the ones used to capture the original images.
