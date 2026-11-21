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
project(ivrole-inventor VERSION 1.0.0 LANGUAGES CXX)

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

# Fail early if one of the required packages cannot be found. The roles
# are built against the header files installed in $MLE_ROOT by
# mle-core-util and mle-core-mlert.
foreach(header mle/mlTypes.h mle/MleRole.h)
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
find_path(SOXT_INCLUDE_DIR Inventor/Xt/viewers/SoXtFullViewer.h)
if (NOT SOXT_INCLUDE_DIR)
  message(FATAL_ERROR "Could not find Inventor/Xt/viewers/SoXtFullViewer.h. Install the SoXt development files.")
endif()

# Specify the compiler macro definitions
add_compile_definitions(
  MLE_DIGITAL_WORKPRINT
  MLE_NOT_UTIL_DLL
  MLE_NOT_MATH_DLL
  MLE_NOT_RUNTIME_DLL
  MLE_NOT_DWP_DLL
  MLE_NOT_3DCOLORMAPMEDIAREF_DLL
  MLE_NOT_3DMODELMEDIAREF_DLL
  MLE_NOT_3DTEXTUREMAPMEDIAREF_DLL
  MLE_NOT_2DROLE_DLL
  MLE_NOT_3DROLE_DLL
  MLE_NOT_PROPERTY_DLL
  MLE_NOT_3DSET_DLL)

# Specify the include directories for the header files
set(ROLES_COMMON_INCLUDE ../../../../common/include)
set(ROLES_INVENTOR_INCLUDE ../../../../inventor/include)
set(PARTS_ROOT ../../../../..)
include_directories(
  ${ROLES_COMMON_INCLUDE}
  ${ROLES_INVENTOR_INCLUDE}
  ${PARTS_ROOT}/sets/inventor
  ${PARTS_ROOT}/mediaref/common/include
  $ENV{MLE_ROOT}/include
  ${COIN_INCLUDE_DIR}
  ${SOXT_INCLUDE_DIR})

set(ROLES_SRC ../../../src)

# The carrier library, libivroles. Like the autotools build, it is not linked
# against the Magic Lantern libraries.
add_library(ivroles SHARED
  ${ROLES_SRC}/MleIv3dCameraCarrier.cxx
  ${ROLES_SRC}/MleIv3dColorMapCarrier.cxx
  ${ROLES_SRC}/MleIv3dModelCarrier.cxx
  ${ROLES_SRC}/MleIv3dTextureMapCarrier.cxx
  ${ROLES_SRC}/MleIv3dTransformCarrier.cxx)
set_target_properties(ivroles PROPERTIES VERSION 1.0.0 SOVERSION 1)
target_compile_definitions(ivroles PRIVATE
  MLE_NOT_3DMODELCARRIER_DLL
  MLE_NOT_3DCOLORMAPCARRIER_DLL
  MLE_NOT_3DTEXTUREMAPCARRIER_DLL
  MLE_NOT_3DTRANSFORMCARRIER_DLL
  MLE_NOT_3DCAMERACARRIER_DLL)

# Add a role module named <name>, built from <source>. The module is not
# linked against the Magic Lantern libraries; the symbols are resolved by the
# player that loads it.
function(add_role name source)
  add_library(${name} MODULE ${source})
  set_target_properties(${name} PROPERTIES PREFIX "")
endfunction()

add_role(Mle2dRole ${ROLES_SRC}/MleIv2dRole.cxx)
target_compile_definitions(Mle2dRole PRIVATE
  MLE_NOT_CARRIER_DLL
  MLE_NOT_2DSET_DLL)

add_role(Mle2dImgRole ${ROLES_SRC}/MleIv2dImageRole.cxx)
target_compile_definitions(Mle2dImgRole PRIVATE
  MLE_NOT_2DIMAGEMEDIAREF_DLL
  MLE_NOT_2DIMAGEROLE_DLL
  MLE_NOT_CARRIER_DLL
  MLE_NOT_IVSTAGE_DLL
  MLE_NOT_2DIMAGEACTOR_DLL)
target_include_directories(Mle2dImgRole PRIVATE
  ${PARTS_ROOT}/sets/inventor/include
  ${PARTS_ROOT}/stages/inventor/include
  ${PARTS_ROOT}/actors/common/include)

add_role(Mle3dRole ${ROLES_SRC}/MleIv3dRole.cxx)
target_compile_definitions(Mle3dRole PRIVATE
  MLE_NOT_3DTRANSFORMCARRIER_DLL
  MLE_NOT_CARRIER_DLL)

# Debug builds are not optimized
foreach(target ivroles Mle2dRole Mle2dImgRole Mle3dRole)
  target_compile_definitions(${target} PRIVATE $<$<CONFIG:Debug>:MLE_DEBUG>)
  target_compile_options(${target} PRIVATE $<$<CONFIG:Debug>:-O0>)
  install(TARGETS ${target}
    LIBRARY DESTINATION lib/inventor)
endforeach()

# Install the header files
install(
  FILES
    ../../../../common/include/mle/2dimgd.h
    ../../../../common/include/mle/2drole.h
    ../../../../common/include/mle/3danimd.h
    ../../../../common/include/mle/3danimrc.h
    ../../../../common/include/mle/3dbboxc.h
    ../../../../common/include/mle/3dcamarc.h
    ../../../../common/include/mle/3dcamc.h
    ../../../../common/include/mle/3dcamclc.h
    ../../../../common/include/mle/3dcamfvc.h
    ../../../../common/include/mle/3dcamtc.h
    ../../../../common/include/mle/3dcharrc.h
    ../../../../common/include/mle/3dcomapc.h
    ../../../../common/include/mle/3dgeomrc.h
    ../../../../common/include/mle/3dgeorwc.h
    ../../../../common/include/mle/3dltac.h
    ../../../../common/include/mle/3dltdc.h
    ../../../../common/include/mle/3dltic.h
    ../../../../common/include/mle/3dltsc.h
    ../../../../common/include/mle/3dlttc.h
    ../../../../common/include/mle/3dmatlrc.h
    ../../../../common/include/mle/3dmodelc.h
    ../../../../common/include/mle/3dnodetc.h
    ../../../../common/include/mle/3dpickc.h
    ../../../../common/include/mle/3drendc.h
    ../../../../common/include/mle/3drgeomc.h
    ../../../../common/include/mle/3drstylc.h
    ../../../../common/include/mle/3dscenec.h
    ../../../../common/include/mle/3dtexmpc.h
    ../../../../common/include/mle/3dtextrc.h
    ../../../../common/include/mle/3dtranrc.h
    ../../../../common/include/mle/3dtransc.h
    ../../../../common/include/mle/Mle2dImageRole.h
    ../../../../common/include/mle/Mle2dRole.h
    ../../../../common/include/mle/Mle3dRole.h
    ../../../../common/include/mle/Mle3dColorMapCarrier.h
    ../../../../common/include/mle/Mle3dModelCarrier.h
    ../../../../common/include/mle/Mle3dTextureMapCarrier.h
    ../../../../common/include/mle/Mle3dTransformCarrier.h
    ../../../../common/include/mle/Mle3dCameraCarrier.h
    ../../../../common/include/mle/paletted.h
    ../../../include/mle/MleIv3dRole.h
  DESTINATION
    include/mle)

# Install the workprints
install(
  FILES
    ../../../../workprints/Mle3dRole.wpd
    ../../../../workprints/Mle2dImgRole.wpd
  DESTINATION
    include/Parts/roles/workprints)

# Uninstall libraries, header files and workprints
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
  TARGET "uninstall"
  POST_BUILD
  COMMENT "Uninstall files with install_manifest.txt"
  COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
          install_manifest.txt to be uninstalled!)
