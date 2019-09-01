# We have to build all our thirdparty libs such that they are deployable.
# This means that we have to make sure that there are no hard-coded dependencies
# in the generated executables (or we use install_name_tool to fix them).
# This script is simply a brute force exercise in building and fixing them,
# also here for future documentation in case of rebuilds.

# Also, had I known how much work this would be, I woulda cmake'd it to generate
# makefiles. Cest la vie.

MAKE_BOOST=false
MAKE_OPENEXR=false
MAKE_ILMBASE=false
MAKE_TBB=false
MAKE_LOG4CPLUS=false
MAKE_BLOSC=false
MAKE_OPENVDB=false
MAKE_GOOGLETEST=false
MAKE_OPENCV=true

# Some simplifying / drying functions
make_link_to_library () {
  FRAMEWORK=$1
  LIBRARY=$2
  TARGET=$3
  pushd $FRAMEWORK &> /dev/null
  ln -Fs Libraries/$LIBRARY $TARGET
  popd &> /dev/null
}

copy_brew_and_otool_lib () {
  FILE=$1
  BASE=$(basename $FILE)
  LIB=$2
  DEST=$3

  sudo install_name_tool -change "$FILE" "@rpath/$BASE" $OCF/OpenCV
  sudo cp $FILE $LGP2
}

if $MAKE_BOOST
then
  # First, prepare boost. We assume at this point that boost is installed
  # with Homebrew. Keep in mind, we're redistributing all dependent libs
  # in the executable; therefore, boost itself will be carried along with
  # the executable, and we just simply automagic the results into the
  # source code. Boost doesn't link to anything that can't be expected to be
  # on every OS X machine.

  echo "Packaging Boost from Homebrew dir"
  BF="Boost.framework"
  BFINC="$BF/Headers"
  BFLIB="$BF/Libraries"
  mkdir -p $BFINC
  cp -r /usr/local/include/boost $BFINC
  mkdir -p $BFLIB
  # OK, here we restrain ourselves to only those needed libraries, to keep distrib of Boost smaller.
  # Those needed by OpenVDB:
  cp -f /usr/local/lib/libboost_iostreams.dylib $BFLIB
  cp -f /usr/local/lib/libboost_system.dylib $BFLIB
  cp -f /usr/local/lib/libboost_filesystem.dylib $BFLIB
  # TODO: The right way to do this is to build boost statically, then use libtool to make any static libraries
  # smooshed into one dynamic library. Instead, I'll just alias one of these as boost, because there
  # needs to be a Boost file in the framework. It's a hack, but much easier.
  make_link_to_library $BF libboost_iostreams.dylib Boost_iostreams
  make_link_to_library $BF libboost_filesystem.dylib Boost_filesystem
  make_link_to_library $BF libboost_system.dylib Boost_system
  make_link_to_library $BF libboost_system.dylib Boost
fi

if $MAKE_OPENEXR
then
  # Assume (as is the convention) that OpenEXR is built locally, but not installed -
  # that is, "make" was run, but not "make install", so the files are in-source.
  echo "Packaging OpenEXR from local install"
  OPENEXR_LOC="../../thirdparty/openexr-2.2.0"
  OEF="OpenEXR.framework"
  OEFINC="$OEF/Headers"
  OEFLIB="$OEF/Libraries"
  mkdir -p $OEFINC
  cp -r $OPENEXR_LOC/include/OpenEXR $OEFINC
  mkdir -p $OEFLIB
  cp -f $OPENEXR_LOC/IlmImf/.libs/libIlmImf.dylib $OEFLIB
  make_link_to_library $OEF libIlmImf.dylib OpenEXR
fi

if $MAKE_ILMBASE
then
  # Ilm is also locally built. In fact, it's a half-sibling with OpenEXR.
  echo "Packaging IlmBase from local install"
  ILMBASE_LOC="../../thirdparty/ilmbase-2.2.0"
  ILF="IlmBase.framework"
  ILFINC="$ILF/Headers"
  ILFLIB="$ILF/Libraries"
  mkdir -p $ILFINC
  cp -r $ILMBASE_LOC/include/OpenEXR $ILFINC
  mkdir -p $ILFLIB
  cp -f $ILMBASE_LOC/Half/.libs/libHalf.dylib $ILFLIB
  make_link_to_library $ILF libHalf.dylib IlmBase
  install_name_tool -id "@rpath/IlmBase.framework/IlmBase" $ILF/IlmBase
fi

if $MAKE_TBB
then
  # ThreadingBuildingBlocks is a concurrency framework, locally built
  echo "Packaging ThreadingBuildingBlocks from local install"
  TBB_LOC="../../thirdparty/tbb44_20151115oss"
  TBF="ThreadingBuildingBlocks.framework"
  TBFINC="$TBF/Headers"
  TBFLIB="$TBF/Libraries"
  mkdir -p $TBFINC
  cp -r $TBB_LOC/include/tbb $TBFINC
  mkdir -p $TBFLIB
  cp -r $TBB_LOC/lib/libtbb.dylib $TBFLIB
  make_link_to_library $TBF libtbb.dylib ThreadingBuildingBlocks
  install_name_tool -id "@rpath/ThreadingBuildingBlocks.framework/ThreadingBuildingBlocks" $TBF/ThreadingBuildingBlocks
fi

if $MAKE_LOG4CPLUS
then
  # Log4cplus is a logging framework, used in OpenVDB
  echo "Packaging Log4CPlus from local install"
  LOG_LOC="../../thirdparty/log4cplus-1.2.0-rc3"
  LGF="Log4CPlus.framework"
  LGFINC="$LGF/Headers"
  LGFLIB="$LGF/Libraries"
  mkdir -p $LGFINC
  cp -r $LOG_LOC/include/log4cplus $LGFINC
  mkdir -p $LGFLIB
  cp -r $LOG_LOC/.libs/liblog4cplus.dylib $LGFLIB
  make_link_to_library $LGF liblog4cplus.dylib Log4CPlus
fi

if $MAKE_BLOSC
then
  # blosc is a dynamic memory compressor, where the processor time used to compress the data
  # is generally recouped by the smaller quantity of data hitting memory and caches, which means
  # it actually speeds up memory access for large datasets.
  echo "Packaging Blosc from local install"
  BLS_LOC="../../thirdparty/c-blosc-1.7.0"
  BLF="Blosc.framework"
  BLFINC="$BLF/Headers"
  BLFLIB="$BLF/Libraries"
  mkdir -p $BLFINC
  cp "$BLS_LOC/blosc/blosc-export.h" $BLFINC
  cp "$BLS_LOC/blosc/blosc.h" $BLFINC
  mkdir -p $BLFLIB
  cp -r "$BLS_LOC/blosc/libblosc.dylib" $BLFLIB
  make_link_to_library $BLF libblosc.dylib Blosc
fi

if $MAKE_OPENVDB
then
  # OpenVDB is hand-built in an annoying and order-dependency unfortunate way.
  # It depends on the above dirs, but will link _directly_ to them as a side effect
  # of how its hand-built Makefile was constructed. Rather than get into a whole
  # rewrite of the makefile, I used install_name_tool to make the binary relocatable
  # with relative paths to its dependencies.
  #
  # Such is the point of a "dist" bundling technique: pragmatic messiness.
  echo "Packaging OpenVDB from local install"
  VDB_LOC="../../thirdparty/openvdb"
  VDF="OpenVDB.framework"
  VDFINC="$VDF/Headers"
  VDFLIB="$VDF/Libraries"
  mkdir -p $VDFINC
  rsync --recursive --include="*/" --include="*.h" --exclude="*" $VDB_LOC $VDFINC
  mkdir -p $VDFLIB
  cp -r $VDB_LOC/libopenvdb.dylib $VDFLIB
  make_link_to_library $VDF libopenvdb.dylib OpenVDB
  install_name_tool -id "@rpath/OpenVDB.framework/OpenVDB" $VDF/OpenVDB
fi

if $MAKE_GOOGLETEST
then
  # For testing, libgtest and libgtest_main need to be available. Thankfully, they are
  # statically linked, which makes things easy.
  # This is a bit strange, packaging googletest in the distributable and _also_ statically
  # linking into the project, but then again, this is just the test target. We don't
  # distribute this with the production target.
  echo "Packaging googletest from local install"
  GTT_LOC="../../thirdparty/googletest/googletest"
  GTF="Googletest.framework"
  GTFINC="$GTF/Headers"
  GTFLIB="$GTF/Libraries"
  mkdir -p $GTFINC
  rsync --recursive --include="*/" --include="*.h" --exclude="*" $GTT_LOC/include/gtest $GTFINC
  mkdir -p $GTFLIB
  cp -r $GTT_LOC/libgtest.a $GTT_LOC/libgtest_main.a $GTFLIB
  make_link_to_library $GTF libgtest.a Googletest
  make_link_to_library $GTF libgtest_main.a Googletest_main
fi

if $MAKE_OPENCV 
then
  # OpenCV can create a "framework". This is, I believe, an iOS framework, and
  # therefore is not based on a dynamic library or even has a framework structure -
  # Essentially, it creates a Unix-like directory structure with a separate
  # Info.plist. Sigh.


  # And that brings us to here.
  echo "Packaging OpenCV framework"
  OCV_LOC="../../thirdparty/opencv-3.4.0/build/install"
  OCV_SHARE="$OCV_LOC/share/OpenCV/3rdparty/lib"
  OCF="OpenCV.framework"
  OCFINC="$OCF/Headers"
  OCFLIB="$OCF/Libraries"
  OCFRSC="$OCF/Resources"
  # The include files go into the include dir
  mkdir -p $OCFINC
  cp -r $OCV_LOC/include/* $OCFINC
  mkdir -p $OCFLIB
  cp $OCV_LOC/lib/libopencv_world.3.4.dylib $OCFLIB/libopencv_world.3.4.dylib
  make_link_to_library $OCF libopencv_world.3.4.dylib OpenCV
  mkdir -p $OCFRSC
  cp -r $OCV_LOC/share/OpenCV/* $OCFRSC
  # Now, adjust the linker symbols
  # Identify the framework
  install_name_tool -id "@rpath/OpenCV.framework/OpenCV" $OCF/OpenCV
  # Copy and adjust baked-in libraries.
  # I could probably adjust these in the OpenCV build, but haven't gotten to it yet.
  # In any case, updating many photo/video Homebrew packages breaks these absolute links,
  # So I'm retooling them to all be relative. It's a brute-force method.

  # libgphoto2
  LGP2="$OCFLIB/gphoto2/lib/"
  mkdir -p $LGP2
  copy_brew_and_otool_lib /usr/local/opt/libgphoto2/lib/libgphoto2.*.dylib $OCF/OpenCV $LGP2
  copy_brew_and_otool_lib /usr/local/opt/libgphoto2/lib/libgphoto2_port.*.dylib $OCF/OpenCV $LGP2
  # This path is set at the time we built opencv. I don't like this hardcoded solution :(
  install_name_tool -rpath "/usr/local/Cellar/libgphoto2/2.5.9/lib" "@loader_path/gphoto2/lib" $OCF/OpenCV
  # ffmpeg
  LFMP="$OCFLIB/ffmpeg/lib/"
  mkdir -p $LFMP
  copy_brew_and_otool_lib /usr/local/opt/ffmpeg/lib/libavcodec.*.dylib $OCF/OpenCV $LFMP
  copy_brew_and_otool_lib /usr/local/opt/ffmpeg/lib/libavformat.*.dylib $OCF/OpenCV $LFMP
  copy_brew_and_otool_lib /usr/local/opt/ffmpeg/lib/libavutil.*.dylib $OCF/OpenCV $LFMP
  copy_brew_and_otool_lib /usr/local/opt/ffmpeg/lib/libswscale.*.dylib $OCF/OpenCV $LFMP
  copy_brew_and_otool_lib /usr/local/opt/ffmpeg/lib/libavresample.*.dylib $OCF/OpenCV $LFMP
  install_name_tool -rpath "/usr/local/Cellar/ffmpeg/3.0.1/lib" "@loader_path/ffmpeg/lib" $OCF/OpenCV

  # This seems to be superfluous based on build dir. Ehh.
  install_name_tool -delete_rpath "/Users/matt/Development/stereo_correlation/thirdparty/opencv-3.4.0/build/install/lib" $OCF/OpenCV
  # I'm going to avoid relocating CUDA since it's probably more dependent on the hardware on this particular machine.
  # Perhaps this isn't correct, but I'll risk it, since it's less likely to update on my dev box (not installed via brew).

fi


