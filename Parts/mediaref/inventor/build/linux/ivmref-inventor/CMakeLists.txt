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
#      http://www.wizzerworks.com
#
# COPYRIGHT_END

# Stop if cmake version is below 3.10
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Project name and enable C++ support
project(ivmref-inventor VERSION 1.0.0 LANGUAGES CXX)

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

# Fail early if one of the required packages cannot be found. The media
# references are built against the header files installed in $MLE_ROOT by
# mle-core-util and mle-core-mlert.
foreach(header mle/mlTypes.h mle/MleMediaRef.h)
  if (NOT EXISTS $ENV{MLE_ROOT}/include/${header})
    message(FATAL_ERROR
      "Could not find $ENV{MLE_ROOT}/include/${header}. "
      "Install mle-core-util and mle-core-mlert into MLE_ROOT first.")
  endif()
endforeach()
find_path(COIN_INCLUDE_DIR Inventor/SoDB.h)
if (NOT COIN_INCLUDE_DIR)
  message(FATAL_ERROR "Could not find Inventor/SoDB.h. Install the Coin development files.")
endif()
find_path(FREEIMAGE_INCLUDE_DIR FreeImage.h)
if (NOT FREEIMAGE_INCLUDE_DIR)
  message(FATAL_ERROR "Could not find FreeImage.h. Install the FreeImage development files.")
endif()

# Specify the include directories for the header files
include_directories(
  ../../../../common/include
  ../../../../inventor
  $ENV{MLE_ROOT}/include
  ${COIN_INCLUDE_DIR}
  ${FREEIMAGE_INCLUDE_DIR})

# Specify the compiler macro definitions
add_compile_definitions(
  MLE_DIGITAL_WORKPRINT
  MLE_NOT_UTIL_DLL
  MLE_NOT_DWP_DLL
  MLE_NOT_RUNTIME_DLL)

# Add a media reference module named <name>, built from <source>.
# The module is not linked against the Magic Lantern libraries; the symbols
# are resolved by the player that loads it.
function(add_mediaref name source)
  add_library(${name} MODULE ${source})
  string(TOUPPER ${name} upper_name)
  string(REGEX REPLACE "^MLE(.*)MEDIAREF$" "\\1MEDIAREF" dll_name ${upper_name})
  set_target_properties(${name} PROPERTIES PREFIX "" OUTPUT_NAME ${name})
  target_compile_definitions(${name} PRIVATE
    MLE_NOT_${dll_name}_DLL
    $<$<CONFIG:Debug>:MLE_DEBUG>)
  target_compile_options(${name} PRIVATE $<$<CONFIG:Debug>:-O0>)
  install(TARGETS ${name} LIBRARY DESTINATION lib/inventor)
endfunction()

add_mediaref(Mle2dImageMediaRef ../../../../inventor/MleIv2dImageMediaRef.cxx)
add_mediaref(Mle3dModelMediaRef ../../../../inventor/MleIv3dModelMediaRef.cxx)
add_mediaref(Mle3dColorMapMediaRef ../../../../inventor/MleIv3dColorMapMediaRef.cxx)
add_mediaref(Mle3dTextureMapMediaRef ../../../../inventor/MleIv3dTextureMapMediaRef.cxx)

# Install the header files
install(
  FILES
    ../../../../common/include/mle/3dcomapm.h
    ../../../../common/include/mle/3dmodelm.h
    ../../../../common/include/mle/3dtexmpm.h
    ../../../../common/include/mle/imagmref.h
    ../../../../common/include/mle/Mle2dImageMediaRef.h
    ../../../../common/include/mle/Mle3dColorMapMediaRef.h
    ../../../../common/include/mle/Mle3dModelMediaRef.h
    ../../../../common/include/mle/Mle3dTextureMapMediaRef.h
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
