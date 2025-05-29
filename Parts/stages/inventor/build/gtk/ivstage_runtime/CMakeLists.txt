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

# Stop if cmake version is below 3.0
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Project name and enable C++ support
project(MleIvStage VERSION 1.0.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify where to find CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Specify Debug postfix
set(CMAKE_DEBUG_POSTFIX "d")

if(POLICY CMP0072)
  # Get rid of OpenGL GLVND warning from CMake 3.11
  cmake_policy(SET CMP0072 NEW)
endif()

# Fail early if one of the required packages cannot be found
find_package(OpenGL REQUIRED)
include(FindGTK2)
find_package(GTK2 REQUIRED gtk)
include(FindCoin3D)
find_package(Coin3D REQUIRED)
include(FindSoGtk)
find_package(SoGtk REQUIRED)
include(FindMagicLantern)
find_package(MagicLantern REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules (
  GTK_GL2    # variable will be used by cmake
  REQUIRED   # is required mandatory
  gtkgl-2.0  # name of package to be found
)

# Specify the library
if(MagicLantern_FOUND)
  message(STATUS "Magic Lantern Variables:")
  message(STATUS "    COIN3D_INCLUDE_DIRS=" ${COIN3D_INCLUDE_DIRS})
  message(STATUS "    COIN3D_LIBRARIES=" ${COIN3D_LIBRARIES})
  message(STATUS "    SoGtk_INCLUDE_DIRS=" ${SoGtk_INCLUDE_DIRS})
  message(STATUS "    SoGtk_LIBRARIES=" ${SoGtk_LIBRARIES})
  message(STATUS "    MagicLantern_INCLUDE_DIRS=" ${MagicLantern_INCLUDE_DIRS})
  message(STATUS "    MagicLantern_LIBRARIES=" ${MagicLantern_LIBRARIES})
  message(STATUS "    MLE_ROOT=" $ENV{MLE_ROOT})
  message(STATUS "    MLE_WORKPRINTS=" $ENV{MLE_WORKPRINTS})

  # Specify the include directories for the header files
  include_directories(
    gen
    ../../../include
    $ENV{MLE_ROOT}/include
    ${MagicLantern_INCLUDE_DIRS}
    ${COIN3D_INCLUDE_DIRS}
    ${SoGtk_INCLUDE_DIRS}
    ${GTK2_INCLUDE_DIRS})

  # Specify the compiler macro definitions
  add_compile_definitions(
    ${MagicLantern_DEFINITIONS}
    MLE_DIGITAL_PLAYPRINT
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
    MLE_SOGTK
    MLE_GTK)

  # Specify the shared library
  add_library(
    MleIvStageShared SHARED
    ../../../src/antirot.cxx
    ../../../src/CubeDragger.cxx
    ../../../src/CubeManip.cxx
    ../../../src/ivstage.cxx
    ../../../src/MultiSearchAction.cxx
    ../../../src/Source.cxx
    ../../../src/SpotMarker.cxx
    ../../../src/Target.cxx)

  # Specify the static library
  add_library(
    MleIvStageStatic STATIC
    ../../../src/antirot.cxx
    ../../../src/CubeDragger.cxx
    ../../../src/CubeManip.cxx
    ../../../src/ivstage.cxx
    ../../../src/MultiSearchAction.cxx
    ../../../src/Source.cxx
    ../../../src/SpotMarker.cxx
    ../../../src/Target.cxx)

  # Specify the shared library properties
  set_target_properties(MleIvStageShared PROPERTIES
    PREFIX ""
    OUTPUT_NAME MleIvStage
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

  target_compile_options(MleIvStageShared
    PRIVATE
      $<$<CONFIG:Debug>:-O0>
      $<$<CONFIG:Release>:>)

  target_compile_definitions(MleIvStageShared
    PRIVATE
      $<$<CONFIG:Debug>: MLE_DEBUG>
      $<$<CONFIG:Release>:>)

  # Specify the static library properties
  set_target_properties(MleIvStageStatic PROPERTIES
    PREFIX ""
    OUTPUT_NAME MleIvStage
    VERSION ${PROJECT_VERSION})

  target_compile_options(MleIvStageStatic
    PRIVATE
      $<$<CONFIG:Debug>:-O0>
      $<$<CONFIG:Release>:>)

  target_compile_definitions(MleIvStageStatic
    PRIVATE
      $<$<CONFIG:Debug>: MLE_DEBUG>
      $<$<CONFIG:Release>:>)

  # Install the libraries
  install(
    TARGETS
      MleIvStageShared
      MleIvStageStatic
    DESTINATION
      lib/mle/runtime
  )

   # Install the header files
  install(
    FILES
      ../../../include/mle/MleIvStage.h
      ../../../include/mle/ivstage.h
    DESTINATION
      include/mle
  )

endif()
