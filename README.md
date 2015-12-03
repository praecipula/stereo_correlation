# Stereo correlation

This project exists because there are no maintained, modern, cross-platform projects for editing and compositing two side by side images into one stereo image of varying formats, particularly with a process oriented toward artistic interpretation. Existing solutions are either overly engineered for a particular task, hard to automate for simple tasks, built for one specific platform and one set of cameras, overly simplistic, or lack certain useful features for content creation.

Theory:
------

The main method of constructing a 3d image is to recreate our vision system's ability to view a scene with a slight displacement between the eyes. By taking a picture with 2 cameras with a slight offset to each other (or, equivalently, if the scene is static, taking 2 pictures, one after the other, with the camera shifted horizontally slightly - the "cha-cha" method), it is possible to provide the 2 images necessary for our eyes to reconstruct the 3D scene.

(Note: you need a set of anaglyph glasses to view the following examples)

![Anaglyph glasses](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a0/3d_glasses_red_cyan.svg/1024px-3d_glasses_red_cyan.svg.png)

![Waimea Falls](https://raw.github.com/praecipula/stereo_correlation/master/example_images/anaglyph_IMG_9098_IMG_8407.png)

**Waimea Falls, Haleiwa, HI**

Motivation:
------

A prototype using Ruby, Qt, ImageMagick, and Cairo was built to serve as a tool to reconstruct anaglyphs from side-by-side point-and-shoot snapshots. One of the original purposes of the Ruby library - to create a "layers and canvas with DSL operations" - was achieved, but at the cost of inconvenience of using toolkits in an awkward way. Specifically, for performance reasons, the Cairo interface was built using [recording surfaces](http://cairographics.org/manual/cairo-Recording-Surfaces.html); these essentially manage a buffer of operations, which when the image is actually rendered, compile and execute lazily the set of instructions. A camera upgrade required that ImageMagick be called, rather than before or after the Cairo steps, right in the middle. The state of ImageMagick on Ruby is only so-so: there are native libraries to handle the interface between the dynamic libraries and the Ruby language, but these are notorious for memory leaks and poor performance, owing to the fact that Ruby doesn't have guaranteed destructors, which is _the_ main way of dealing with resource lifetime management in C++. It's too easy to accidentally allocate dynamic memory without knowing you did so. Calling ImageMagick with command-line exec statements, another common approach, requires either the generation of a command-line statement composing library and is not cross-platform, or using a library like RMagick to manage the interaction, but workflows of this type almost always incur multiple image buffers being written to disk as tempfiles.

This project deserved a native implementation. I decided to port over to C++, which I'm quite familiar with having worked for years as a native app software guy, for direct access to the tools and libraries used to make this project work. Features at time of completion will be:


* 2-image correlation of the left and right images (visual alignment)
* Basic rotation and skew transforms (camera rig error correction)
* Composable or programmable pipeline (construct a processing DAG similar to Unreal's [Blueprints](https://docs.unrealengine.com/latest/INT/Engine/Blueprints/index.html) for scripting workflow functionality)
* Red-cyan anaglyph generation as a possible output format
* Enhanced (color-reconstituted) anaglyph as a possible output format
* Wigglegram generation as a possible output format
* JPS file generation as a possible output format
* Crop and resize capability as a pipeline step - trimming uncorrelated border pixels between images
* Floating frame generation as a pipeline step, to fix edge violations for foreground geometry
* Annotation and signature as a pipeline step, just to say the author was here
* Lens undistort as a pipeline step, allowing for better camera callibration.
* Automated correlation finding, to provide suitable "candidates" for selection of the view frame point - the point of exact pixel coordinate overlap of the geometry captured in left and right images.


Stretch features:
* Batch processing of frames from side-by-side captured movies for generation of 3D movies.
* Generation of 3D scene from disparity data (construct geometry which allows small amounts of rotation and translation of the recreated scene for a "shadow box" effect)

Project status:
------

I have just begun my port into C++ in my spare time, so there are definitely some things lacking. Documentation (other than this overview) and testing are quite poor, to my standards, in terms of completion - documentation is just dev notes, and testing is a few meager unit tests and hand-comparison to known good outputs from the prototype. This is not due to a lack of caring or a lack of awareness, but rather an attempt to recreate this project at a similar completion level as the prototype, which will allow me to focus on the native implementation for my uses.

Documentation will be handled with Doxygen, testing is using google test.

A deeper look into the features
------

* 2-image correlation of the left and right images

Given a view which displays the left and the right image as taken by the camera, it is possible to define the apparent depth of the image plane in the scene by the selection of how the images are displaced relative to each other. The point on the image where both features overlap precisely - that is, the pixel coordinate at which the exact same spot lies in the real-world geometry; the features are perfectly superimposed from both images - this point is the apparent location of the image plane. It's the "screen-level" depth of the image to the viewer, which allows the compositor to choose whether items in the scene pop out of the screen or descend into it. Using a mouse-based selection of this point on each image, we can calculate the operations needed to properly align and compose the images on top of one another.

Status in prototype: 100% (fully working)

Status in native: 75% (the view and basic UI exist, but the data model is in process and no operations are yet done to correlate the images together).

* Basic rotation and skew transforms

Constructing 2 lenses side-by-side, whether on one housing or with twinned cameras, has a problem: it's very difficult to build a rig that has precise tolerances which will result in perfect image alignment. Slight manufacturing or mounting issues can result in an image which has one camera slighty rotated relative to the other; additionally, except where very highly toleranced lenses are used, the images may inherently have different pincushion/barrel and skew distortions. A set of basic transforms to align other points in the image (other than the point of overlap) can correct for these real-world phenomenon. This can be some sort of functional transform, like a [cubic lens distortion](http://www.imagemagick.org/Usage/distorts/#barrel), or a manual point-by-point [control point distortion](http://www.imagemagick.org/Usage/distorts/#control_points) to really get fine control of the results.

Status in prototype: 0% (relied on strong camera rig construction; slight rotations of 1-2 degrees done in preprocessing by hand when necessary)

Status in native: 0%

* Composable or programmable pipeline

It's possible to have different workflows for images: for instance, we may want to have a floating frame to correct for edge violations for a certain image, or we may not, or we may want to undistort lens parameters at a certain step in the processing pipeline (i.e. before or after alignment). This feature revolves around using the [command pattern](https://en.wikipedia.org/wiki/Command_pattern) in order to allow a user-composeable pipeline of operations that chain to form the complete workflow. A DAG is constructed, such that the head nodes could be, for example, opening a left and right image; these are then combined to an overlay node, framed with a floating frame node, forked to an anaglyph and a SBS image processing set of sibling nodes, and output to a JPG and JPS file respectively. This DAG is intended to be more or less the serializable canonical model used in the program; opening a serialized file of this DAG will cause the program to be able to precisely recreate final results by reprocessing the workflow.

There is another wrinkle present in constructing this workflow diagram; there are no good UI interfaces for visually constructing a DAG that are available cross-platform (i.e. in Qt). Since I have to roll my own, I took inspiration from [Unity's Blueprint scripting engine](https://docs.unrealengine.com/latest/INT/Engine/Blueprints/index.html) for the user experience, to be implemented in OpenGL, as most of the visuals are. Of course, implementing such a UI in OpenGL is not as trivial a task as it may seem; OpenGL offers no font rendering support; it's one of the few times in this program we need to use OpenGL cursor hit detection; there's a fairly normal, but unique in the program, amount of management of view space (i.e. zooming, panning) that needs to happen. Of course, since this program is not necessarily expected to be used by anyone other than myself, I get to build it however I want. YAGNI is always relaxed on personal projects.

For handling fonts, I'm implementing Valve's [distance field font](http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf) concept. I have of course dealt with texture-based font atlases; however, Valve's implementation is extremely clever and provides essentially hardware-accelerated runtime antialiasing. The constructed DAG of nodes uses a [Boost::Graph](http://www.boost.org/doc/libs/1_59_0/libs/graph/doc/index.html) DAG with [Boost::Serialize](http://www.boost.org/doc/libs/1_59_0/libs/serialization/doc/index.html) "workspace" implementation and plugins that correspond to an API designed for future extensions and new operations. The completion of this feature is a major driving force for rewriting the prototype into a bona-fide native app, driven by the flaws in the prototype's image processing pipeline.

Status in prototype: 0% (not prototyped, the workflow is reprogrammed by hand for each case)

Status in native: 20% (a single command has been built and "hack-tested" to prove the concept and establish the api; ui space created for this purpose; font atlas generator created with serializable metadata for program use.)

* Red-cyan anaglyph generation

Anaglyph glasses are those stereotypical 3D "color filter" glasses with a red lens on the left and blue lens on the right. We can isolate the red chanel on the right image and the cyan (green and blue) color on the left image by multiplying with pure red (#FF000) and pure cyan (#00FFFF) to essentially do a bitwise-AND of only the relevant image data. We can then use the compositing operator "screen" to combine the red chanel into the blue image (or vice versa, screen is commutative), which has the effect of pulling the appropriate channels in while lightening the result. This generates an anaglyph image, which is the preferred method of generating static (as opposed to, e.g., a wigglegram) and device-independent (as opposed to requiring special equipment) 3D images, despite the drawback of not retaining true color and having issues with near-red and near-cyan elements in the image.

![Koko Head](https://raw.github.com/praecipula/stereo_correlation/master/example_images/anaglyph_IMG_9222_IMG_8530.png)

**Koko Head, HI. Notice my friend's teal shirt violently "vibrates" as an anaglyph; the right eye sees it as light blue, the left eye sees black.**

Status in prototype: 100%

Status in native: 0%

* Enhanced anaglyph generation

A huge downside to using the standard anaglyph workflow as described above is that the first image processing step is to slice some of the color information out of the image; this results in some near-red source color and near-cyan source color issues when viewed by a human, as described above. Rather than performing a bitwise AND of the source image data, it would be better to implement a filter which takes into account the energy of the correlated pixels _post-slice_, and reclaim some information from the sliced bits in order to prevent the cross-eye vibration artifacts present in anaglyph images. A simple filter is to modify the entire image on either side; this will in general, of course, have an effect over the entire image, which is not desirable. A better algorithm could be to examine pixel neighborhoods at similar disparity levels which seem to match and see if the luminosity for each eye is within an acceptable bounds; this is of course dependent upon the construction of the disparity map of the image as presented below. My a priori approach to properly implement this filter is to examine the disparity map (described below in the "reconstructing 3D geometry" section) for overlaps with strongly uncorrelated luminosity between images; these images will be re-examined in their original form for contour selection with OpenCV, then the contoured areas (potentially filtered for noise) with the most luminosity loss post-anaglyph will be modified from the original such that the resultant anaglyph has (closer) visual matching. The net effect will be that the shirt that appears _strongly_ blue (or nearly synonymous with white) in the cyan image, but which appears nearly black in the red image, will cause less visual vibration - it will be softened in the red image to a gray. Potential downsides to this could be that the human eye may compensate for this disparity due to our innate filtering ability; the result may be some sort of visual purple in the anaglyph. In this case, a new decision about how to process this region will have to be created.

This is the most complex operation of the entire program, dependent on not only a decent displacement matching algorithm, but also a decent workflow in itself to implement the results. Art is pain, I guess.

* Wigglegram generation

A wigglegram is an animated gif of the two images oscillating back and forth. [Here](https://www.flickr.com/photos/rosendahl/4720261964/sizes/o/) is an example (photo credit Flickr user rosendahl). This gives some sense of depth to the image based purely on our brain's ability to process parallax, and preserves the original coloration which is not possible in anaglyph images. It can be displayed on a computer or tablet that does not have specialized hardware for displaying 3D images, especially in situations where the original color retention is important. This simplistic construction method can be vastly improved; read below for more details on the 3D image geometry reconstruction feature.

Status in prototype: 0%

Status in native: 0%

* JPS file generation

A JPS file is a file type for stereoscopic JPEG images. It is essentially a side-by-side construction of 2 JPEG images packed into one file, with some metadata markers that describe the packing of the images. 3d-aware viewing devices (like my own polarized-glasses based 3D television) can often show these images natively in their galleries, providing a convenient and portable container for trading images. Some software can construct an anaglyph on the fly to display JPS files on a non-3d-enabled viewing platform, though of course, success in the enhanced anaglyph image as described about will likely be preferable.

Status in prototype: 0%

Status in native: 0%

* Crop and resize capability as a pipeline step

These are basic functions that are useful for composition of an image outside of the camera. When viewing 3d images in a gallery, especially when 3d techniques such as a floating frame are used, the images tend to jump around from size to size, which is distracting. To correctly resize the images to appear of the same size requires a resize and crop functionality that operates on the images themselves early in the processing chain, before overlapping and floating frame operations have been applied, as these later steps provide extra padding horizontally, but not vertically, which alter the ability of other tools to successfully resize the images.

Status in prototype: 40% (basic crop and resize work, but are limited in where and how they can be applied).

Status in native: 0%

* Floating frame generation as a pipeline step

A floating frame is a technique to compensate for edge violation. When an image extends in front of the view plane (popping out at the viewer), if a feature crosses the left and right side of the frame, it is immediately obvious and uncomfortable to the viewer. One eye still sees much of the object, where another eye gets blocked by the edge of the frame. This effect does not happen on the top and bottom of the image, because our eyes are mounted side-by-side, leading to displacements that only violate at the left and right edge of the frame. By floating a frame in front of the view plane, that is, placing a frame around the images and then displacing it with the foreground geometry, then allowing the frame to appear in front of the view plane with its own margin, we can cut the images not at the view plane (where the true edge of the image is), but at the frame's apparent depth, or height. The effect is to push a frame out to the viewer at the same distance as the frontmost geometry, which corrects the edge violation.

![Hanauma Bay](https://raw.github.com/praecipula/stereo_correlation/master/example_images/anaglyph_IMG_9026_IMG_8334.png)

**Hanauma Bay. Notice the _strongly_ floated frame to correct for edge violation of rock wall in the bottom right corner.**

Status in prototype: 100%

Status in native: 0%

* Annotation and signature as a pipeline step

When we have a frame, we might as well use it to title and annotate the image with text, as well as apply an artist's signature. To do this correctly, we have to float the annotation with the frame, meaning we have to apply the annotation separately to each frame before the final displacement happens. This pushes the text out of plane at the same displacement as the frame pushes out of plane, which means the signature appears on the surface of the frame.

Status in prototype: 80% (was working, but broken by regression)

Status in native: 0%

* Lens undistort as a pipeline step

My 3D camera setup has changed; originally, it was 2 point and shoot cameras that had fairly rectilinear lenses with only slight barrel distortion, which was acceptable to leave in the image when viewed by humans. The downside to this setup was the interocular distance of the lenses was 64mm, which is right about where the interpupillary distance of the average American man sits (64.7). Since the cameras cannot "toe in" in the same way that human eyes can, this means that there was a strict comfortable distance to the nearest object in the scene (or more correctly, a strict distance _ratio_ between the nearest and furthest object at which disparities get uncomfortable). To display correctly, everything in the scene must be seen without any eye crossing; the viewer's eyes have to replicate the near-parallel camera setup; the near point of the scene was around 1.5-2m for correct capture.

I recently upgraded to 2 GoPros in order to decrease the interocular distance, to around 40mm. This allows closer images to be taken, but has the obvious downside of a highly distorted image due to the GoPro's aspherical lens. There are good undistort tools available in ImageMagick, and even better in OpenCV with calibration images, but I determined experimentally that the undistort operation must occur at a specific point in the pipeline in order to produce the best results: after image correlation and overlapping, but before superimposing. This causes the distortion to affect the images relatively uniformly, causing any problems in the undistort model (cubic barrel distortion) to be corrected with the images "sticking" on top of each other. Undistortion prior to correlation can cause excess distortion toward the edges of the image, especially when the image plane point is off-center. This is one of the drivers for an explicit image pipeline to exist: correlate the images, then undistort to rectify geometry.

Status in prototype: 0% (experimentation was done with general purpose image manipulation tools)

Status in native: 0% (it's as yet unclear whether an undistort can be done to be visually and computationally satisfying, or whether GoPro images can even be universally rectified using barrel distortions. It may be possible that the aspherical lenses need different distortions at different depths from the lens; the most expedient action here would to be to use a 3rd-party lens retrofit for the GoPros to give them a more rectilinear image capture.)

* Automated correlation finding (edge detection and correlation)

Having a correlation point that ties the images together and defines the view plane is one of the many artistic choices that can be made when compositing the images, but the process of finding such a point can be tedious. Pixel-level precision requires zooming into the image to accurately place the correlation point; but detail is lost to the eye at close approach. A method to assist in selecting correlation points would be of great benefit to the workflow. OpenCV has some built-in tools for corner finding and feature detection that could provide a reasonable candidate set between images which would be of higer precision than requiring the user to zoom all the way in to a pixel-level detail.

Status in prototype: 0%

Status in native: 0%

* Batch processing of frames from side-by-side movies for generation of 3d movies.

All of the above features, when applied to a movie, with additional time-based tweening of some of the parameters (i.e. I've always been interested to determine if a dolly-zoom like effect can be built into a movie by tweening the view plane, which will push or pull the entire scene to the viewer). Essentially, allow keyframe-based, tweened model adjustments which bring many of the above features into the time domain.

* Generation of 3d scene from correlation data (construct geometry in OpenGL which allows small amounts of rotation and translation of the recreated scene)

If enough points of correlation between images can be found, the degree of horizontal shift (displacement in pixels) for each point can determine the degree of displacement from the image plane. In other words, a correlation can be seen as the intersection of two vectors projecting from the two cameras; the projection of one vector on the other camera defines an epiline, the line which represents a ray projected through Camera A's view plane as it appears on Camera B's image. Finding the point along this line where Camera B sees the same geometry defines a precise 3D point. When that point is found, the disparity along the epiline to the point of intersection can be triangulated to define the 3-dimensional location of the point.

Assuming a model where 2 calibrated cameras are planar and separated by their interocular distance, the epilines are horizontal, and knowing information about the cameras (i.e. the interocular distance) can enable us to calculate a depth distance for each point. These data can be used to create a point cloud, which can be meshed and textured with the images in order to recreate a 3d scene that can be slightly panned and tilted to recreate novel views of the original scene, within the limits of the original cameras to capture (features occluded in one or both images will, of course, yield no color or depth information).

Using these assumptions, some algorithms have been developed for stereo correspondance map finding. OpenCV has [some approaches that operate fairly well](http://docs.opencv.org/master/dd/d47/group__cudastereo.html#gsc.tab=0) for this purpose, including reprojecting to 3D space. The algorithm for generating the disparity map in our case will be [belief propagation](http://docs.opencv.org/master/de/d7a/classcv_1_1cuda_1_1StereoBeliefPropagation.html#gsc.tab=0), a technique that (as in many computer vision problems) begins by modeling the images as a [hidden markov model](https://en.wikipedia.org/wiki/Hidden_Markov_model), i.e. the status of a particular pixel influences and is influenced by its direct neigbors' status. Status in this case means the hidden variable of disparity, or a measurement of how far a particular feature in 3D space is shifted between cameras' view planes. Belief propagation relies on a message passing algorithm to send beliefs, or a set of probabilities, between neighbors in order to converge on a most-likely belief for where the disparity of each pixel lies. For a humanistic interpretation, essentially the pixels "gossip" at each other to encourage slightly-misguided neighbors to comply with the group consensus. This will be the selected algorithm because it tends to do a good job while remaining extremely parallelizable, and therefore performant - OpenCV has a modified implementation of belief propagation in its cuda library, which means that the algorithm is run on the graphics processor and is therefore almost real-time.

Given the disparity map, we will construct a 3D point cloud (again [OpenCV](http://docs.opencv.org/master/dd/d47/group__cudastereo.html#gaff851e3932da0f3e74d1be1d8855f094) helps us here) and use that to build a mesh of the scene. The meshing technique will be [marching cubes](https://en.wikipedia.org/wiki/Marching_cubes), which OpenCV does not appear to implement - it will have to be built by myself. The point cloud is predicted to be fairly noisy, or, more precisely (after the belief propagation model is applied) not necessarily locally noisy but quite inexact and lacking any sort of fine detail. In this situation, an isosurface representation, where we model the points as noisy repressentations of a fundamental "truth" surface modeled by a function, should fit the data just fine. Computational geometry - simply tesselating between the points, say - can't handle noise as well as isosurface methods, and stereo correlation point clouds are incredibly noisy and jagged.

Since there will be no lighting applied to the 3D model in the render stage (this is simply for parallax-based or very-slightly-perspective-shifted interactions) we don't have to worry as much about the [angular artifacts](https://en.wikipedia.org/wiki/Marching_cubes#/media/File:Marchingcubes-head.png) that often occur with marching cubes. In addition, the point cloud will be so softened as to preclude, for example, a [poisson surface reconstruction](http://research.microsoft.com/en-us/um/people/hoppe/proj/poissonrecon/), which led to the choice of marching cubes as the simpler and more computationally efficient algorithm.
