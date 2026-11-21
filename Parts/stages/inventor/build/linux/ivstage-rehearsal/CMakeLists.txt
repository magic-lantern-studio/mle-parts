# COPYRIGHT_BEGIN
#
# The MIT License (MIT)
#
# Copyright (c) 2025 Wizzer Works
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#  For information concerning this header file, contact Mark S. Millard,
#  of Wizzer Works at msm@wizzerworks.com.
#
#  More information concerning Wizzer Works may be found at
#
#  For information concerning this header file, contact Mark S. Millard,
#  of Wizzer Works at msm@wizzerworks.com.
#
#  More information concerning Wizzer Works may be found at
#
#      http://www.wizzerworks.com
#
# COPYRIGHT_END

# Stop if cmake version is below 3.10
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Project name and enable C++ support
project(ivstage-rehearsal VERSION 1.0.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify Debug postfix
set(CMAKE_DEBUG_POSTFIX "d")

# Check for Magic Lantern environment variables
if (DEFINED ENV{MLE_ROOT})
  message(STATUS "MLE_ROOT=" $ENV{MLE_ROOT})
else()
  message(STATUS "MLE_ROOT NOT SET")
  set(ENV{MLE_ROOT} "/opt/MagicLantern")
  message(STATUS "Using " $ENV{MLE_ROOT})
endif()

# Fail early if one of the required packages cannot be found. The stage
# is built against the header files installed in $MLE_ROOT by
# mle-core-util, mle-core-mlert and the mle-parts roles and sets.
foreach(header mle/mlTypes.h mle/MleStage.h mle/Mle3dRole.h mle/Mle3dSet.h)
  if (NOT EXISTS $ENV{MLE_ROOT}/include/${header})
    message(FATAL_ERROR
      "Could not find $ENV{MLE_ROOT}/include/${header}. "
      "Install mle-core-util, mle-core-mlert and the mle-parts roles and sets into MLE_ROOT first.")
  endif()
endforeach()
find_path(COIN_INCLUDE_DIR Inventor/SoDB.h)
if (NOT COIN_INCLUDE_DIR)
  message(FATAL_ERROR "Could not find Inventor/SoDB.h. Install the Coin development files.")
endif()
find_path(SOXT_INCLUDE_DIR Inventor/Xt/viewers/SoXtFullViewer.h)
if (NOT SOXT_INCLUDE_DIR)
  message(FATAL_ERROR "Could not find Inventor/Xt/viewers/SoXtFullViewer.h. Install the SoXt development files.")
endif()

# Specify the compiler macro definitions
add_compile_definitions(
  MLE_REHEARSAL
  MLE_DIGITAL_WORKPRINT
  MLE_NOT_UTIL_DLL
  MLE_NOT_MATH_DLL
  MLE_NOT_RUNTIME_DLL
  MLE_NOT_DWP_DLL
  MLE_NOT_PROPERTY_DLL
  MLE_NOT_2DROLE_DLL
  MLE_NOT_3DROLE_DLL
  MLE_NOT_2DSET_DLL
  MLE_NOT_3DSET_DLL
  MLE_NOT_IVSTAGE_DLL
  $<$<CONFIG:Debug>:MLE_DEBUG>)

# Specify the include directories for the header files
set(STAGES_INVENTOR ../../..)
include_directories(
  ${STAGES_INVENTOR}/include
  $ENV{MLE_ROOT}/include
  ${COIN_INCLUDE_DIR}
  ${SOXT_INCLUDE_DIR})

# The stage module. It is not linked against the Magic Lantern libraries; the
# symbols are resolved by the player that loads it.
add_library(MleIvStage MODULE
  ${STAGES_INVENTOR}/src/antirot.cxx
  ${STAGES_INVENTOR}/src/CubeDragger.cxx
  ${STAGES_INVENTOR}/src/CubeManip.cxx
  ${STAGES_INVENTOR}/src/ivstage.cxx
  ${STAGES_INVENTOR}/src/MultiSearchAction.cxx
  ${STAGES_INVENTOR}/src/Nudger.cxx
  ${STAGES_INVENTOR}/src/Snapper.cxx
  ${STAGES_INVENTOR}/src/Source.cxx
  ${STAGES_INVENTOR}/src/SpotMarker.cxx
  ${STAGES_INVENTOR}/src/Target.cxx)
set_target_properties(MleIvStage PROPERTIES PREFIX "")
target_compile_options(MleIvStage PRIVATE $<$<CONFIG:Debug>:-O0>)
install(TARGETS MleIvStage LIBRARY DESTINATION lib/rehearsal)

# Install the header files
install(
  FILES
    ../../../include/mle/MleIvStage.h
    ../../../include/mle/ivstage.h
  DESTINATION
    include/mle)

# Uninstall libraries and header files
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
  TARGET "uninstall"
  POST_BUILD
  COMMENT "Uninstall files with install_manifest.txt"
  COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
          install_manifest.txt to be uninstalled!)
