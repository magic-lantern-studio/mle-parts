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
project(ivactor_rehearsal VERSION 1.0.0 LANGUAGES CXX)

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

# Fail early if one of the required packages cannot be found. The actors
# are built against the header files installed in $MLE_ROOT by
# mle-core-util, mle-core-mlert and the mle-parts roles.
foreach(header mle/mlTypes.h mle/MleActor.h mle/3dcamc.h mle/2dimgd.h)
  if (NOT EXISTS $ENV{MLE_ROOT}/include/${header})
    message(FATAL_ERROR
      "Could not find $ENV{MLE_ROOT}/include/${header}. "
      "Install mle-core-util, mle-core-mlert and the mle-parts roles into MLE_ROOT first.")
  endif()
endforeach()

# Specify the compiler macro definitions
add_compile_definitions(
  MLE_REHEARSAL
  MLE_DIGITAL_WORKPRINT
  MLE_NOT_UTIL_DLL
  MLE_NOT_MATH_DLL
  MLE_NOT_RUNTIME_DLL
  MLE_NOT_DWP_DLL
  $<$<CONFIG:Debug>:MLE_DEBUG>)

# Specify the include directories for the header files
set(ACTORS_COMMON ../../../../common)
include_directories(
  ${ACTORS_COMMON}/include
  $ENV{MLE_ROOT}/include)

# Add an actor module named <name>, built from <source>. The module is not
# linked against the Magic Lantern libraries; the symbols are resolved by the
# player that loads it.
function(add_actor name source)
  add_library(${name} MODULE ${source})
  set_target_properties(${name} PROPERTIES PREFIX "")
  target_compile_options(${name} PRIVATE $<$<CONFIG:Debug>:-O0>)
  install(TARGETS ${name} LIBRARY DESTINATION lib/rehearsal)
endfunction()

add_actor(Mle2dImageActor ${ACTORS_COMMON}/src/2dimga.cxx)
target_compile_definitions(Mle2dImageActor PRIVATE
  MLE_NOT_2DROLE_DLL
  MLE_NOT_2DIMAGEROLE_DLL
  MLE_NOT_2DIMAGEACTOR_DLL)

add_actor(Mle3dCameraActor ${ACTORS_COMMON}/src/3dcama.cxx)
target_compile_definitions(Mle3dCameraActor PRIVATE
  MLE_NOT_3DCAMERACARRIER_DLL
  MLE_NOT_3DCAMERAACTOR_DLL)

add_actor(MleEscapeActor ${ACTORS_COMMON}/src/escapea.cxx)
target_compile_definitions(MleEscapeActor PRIVATE
  MLE_NOT_CARRIER_DLL
  MLE_NOT_ESCAPEACTOR_DLL
  MLE_XT)

# Install the header files
install(
  FILES
    ../../../../common/include/mle/escapea.h
    ../../../../common/include/mle/MleEscapeActor.h
    ../../../../common/include/mle/3dcama.h
    ../../../../common/include/mle/Mle3dCameraActor.h
    ../../../../common/include/mle/2dimga.h
    ../../../../common/include/mle/Mle2dImageActor.h
  DESTINATION
    include/mle)

# Install the workprints
install(
  FILES
    ../../../../workprints/Mle3dCameraActor.wpd
    ../../../../workprints/Mle3dCameraActor.wpa
    ../../../../workprints/MleEscapeActor.wpd
    ../../../../workprints/MleEscapeActor.wpa
    ../../../../workprints/Mle2dImgActor.wpd
    ../../../../workprints/Mle2dImgActor.wpa
  DESTINATION
    include/Parts/actors/workprints)

# Uninstall libraries, header files and workprints
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
  TARGET "uninstall"
  POST_BUILD
  COMMENT "Uninstall files with install_manifest.txt"
  COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
          install_manifest.txt to be uninstalled!)
