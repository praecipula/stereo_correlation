Distributatble files
------

These distributable directories contain all the necessary libraries to be repackaged with stereo_correlation for each varied platform. They are intended to be built out-of-source by some maintainer, though in the relevant platform directories, there are scripts and/or project files that show how a particular set was built. In some cases, for example, it's prudent and acceptable to assume that a package manager did a good thing (verified by the maintainer of the distributables) and the files can be simply copied - this is true of, e.g. Boost with homebrew on OS X, which only depends on the paths of standard c++ libraries assumed to be on every Mac.

The files are intended to be built and maintained more or less "by hand" and checked into the repository for ease of development, as opposed to a "rebuild the world from source" approach, so if the build fails for a particular platform due to unmet dependencies, well, it's a fun, brave new world to build the distributables that has been discovered by the poor soul who hits that condition.
