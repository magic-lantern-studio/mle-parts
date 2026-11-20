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

# Project name. The props part only installs header files and workprints,
# so no compiler is needed.
project(MleProps VERSION 1.0.0 LANGUAGES NONE)

# Install to /opt/MagicLantern unless -DCMAKE_INSTALL_PREFIX is given.
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/opt/MagicLantern" CACHE PATH
      "Install path prefix" FORCE)
endif()

# Property header files.
set(MLE_PROPS_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3danimrp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dcamarp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dcamclp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dcamfvp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dcamtp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dcharrp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dcomapp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3derotp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dgeomrp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dgeorwp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dltap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dltdp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dltip.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dltsp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dlttp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dmatlrp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dmodelp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dnodetp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dnscalp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dpickp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dqrotp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3drendp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3drgeomp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3drstylp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dscenep.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dtexmpp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dtextrp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dtranfp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dtranlp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3dtranrp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/3duscalp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/myprop1p.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/myprop2p.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/myprop3p.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/include/mle/propdef.h
)

# Property workprints.
set(MLE_PROPS_WORKPRINTS
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dCameraFieldOfViewProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dCameraTypeProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dCharacterRegistryProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dColorMapProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dEulerAngleRotationProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dGeometryRegistryProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dGeometryRegistryWebProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dLightAngleProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dLightDropOffRateProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dLightIntensityProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dLightSwitchProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dLightTypeProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dMaterialRegistryProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dModelProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dNodeTypeProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dNonuniformScaleProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dOrientationProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dPickableProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dPositionProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dQuaternionRotationProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dRehearsalGeometryProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dRenderProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dRenderStyleProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dSceneGraphProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dTextureMapProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dTextureRegistryProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dTransformProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dTranslationProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dUniformScaleProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dAnimationRegistryProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dCameraAspectRatioProperty.wpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../workprints/Mle3dCameraClippingProperty.wpp
)

install(FILES ${MLE_PROPS_HEADERS} DESTINATION include/mle)
install(FILES ${MLE_PROPS_WORKPRINTS} DESTINATION include/workprints)

# Uninstall header files and workprints
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
  TARGET "uninstall"
  POST_BUILD
  COMMENT "Uninstall files with install_manifest.txt"
  COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
          install_manifest.txt to be uninstalled!)
