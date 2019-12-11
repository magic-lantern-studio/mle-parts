# -*- Makefile -*-
#
#  Copyright (C) 2000-2007  Wizzer Works
#
#  Wizzer Works makes available all content in this file ("Content").
#  Unless otherwise indicated below, the Content is provided to you
#  under the terms and conditions of the Common Public License Version 1.0
#  ("CPL"). A copy of the CPL is available at
#
#      http://opensource.org/licenses/cpl1.0.php
#
#  For purposes of the CPL, "Program" will mean the Content.
#
#  For information concerning this Makefile, contact Mark S. Millard,
#  of Wizzer Works at msm@wizzerworks.com.
#
#*************************************************************************
#
# Makefile for installing header files.
#
# $Id$
#
###########################################################################

ISM_DEPTH = ..
include $(ISM_DEPTH)/ismdefs

VPATH = $(ISM_DEPTH)/../workprints

WORKPRINTS = \
	Mle3dCameraFieldOfViewProperty.wpp \
	Mle3dCameraTypeProperty.wpp \
	Mle3dCharacterRegistryProperty.wpp \
	Mle3dColorMapProperty.wpp \
	Mle3dEulerAngleRotationProperty.wpp \
	Mle3dGeometryRegistryProperty.wpp \
	Mle3dGeometryRegistryWebProperty.wpp \
	Mle3dLightAngleProperty.wpp \
	Mle3dLightDropOffRateProperty.wpp \
	Mle3dLightIntensityProperty.wpp \
	Mle3dLightSwitchProperty.wpp \
	Mle3dLightTypeProperty.wpp \
	Mle3dMaterialRegistryProperty.wpp \
	Mle3dModelProperty.wpp \
	Mle3dNodeTypeProperty.wpp \
	Mle3dNonuniformScaleProperty.wpp \
	Mle3dOrientationProperty.wpp \
	Mle3dPickableProperty.wpp \
	Mle3dPositionProperty.wpp \
	Mle3dQuaternionRotationProperty.wpp \
	Mle3dRehearsalGeometryProperty.wpp \
	Mle3dRenderProperty.wpp \
	Mle3dRenderStyleProperty.wpp \
	Mle3dSceneGraphProperty.wpp \
	Mle3dTextureMapProperty.wpp \
	Mle3dTextureRegistryProperty.wpp \
	Mle3dTransformProperty.wpp \
	Mle3dTranslationProperty.wpp \
	Mle3dUniformScaleProperty.wpp \
	Mle3dAnimationRegistryProperty.wpp \
	Mle3dCameraAspectRatioProperty.wpp \
	Mle3dCameraClippingProperty.wpp \
	$(NULL)

install: $(WORKPRINTS)
	$(INSTALL) -d $(MLE_ROOT)/include/workprints
	$(INSTALL) $^ $(MLE_ROOT)/include/workprints

uninstall: $(WORKPRINTS)
#	cat install_manifest.txt | xargs echo sudo rm -f | sh
	cat install_manifest.txt | xargs echo rm -f | sh

include $(ISMRULES)

collect:
	$(IBCOLLECT) -baseDir $(ISM_DEPTH)/.. \
                     -log ../build/collect.raw \
                     -srcDir $(ISM_DEPTH)/../workprints \
                     -destDir "include/workprints" \
                     -tags "INCLUDE" \
                     $(WORKPRINTS)
