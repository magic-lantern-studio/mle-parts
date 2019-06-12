/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dCameraCarrier.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for a 3D Camera interface to a Set
 * targeting the Inventor platform.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <stdio.h>

#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSwitch.h>

#include <math/vector.h>
#include <math/rotation.h>

#include "mle/iv3dset.h"

#include <mle/3dcamc.h>

int
Mle3dCameraCarrier::setPosition(MleSet *f,const MlVector3 *position)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	// Set the position.
	camera->position.setValue(
		mlScalarToFloat((*position)[0]),
		mlScalarToFloat((*position)[1]),
		mlScalarToFloat((*position)[2]));
	
	return 0;	// no error
}

MlVector3
Mle3dCameraCarrier::getPosition(MleSet *f)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	MlVector3 position;

	SbVec3f p = camera->position.getValue();

	position[0] = mlFloatToScalar(p[0]);
	position[1] = mlFloatToScalar(p[1]);
	position[2] = mlFloatToScalar(p[2]);

	return position;
}

int
Mle3dCameraCarrier::setOrientation(MleSet *f,const MlRotation *orientation)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	// Set the orientation.
	MlScalar w,x,y,z;
	orientation->getValue(w,x,y,z);
	camera->orientation.setValue(
		mlScalarToFloat(w),
		mlScalarToFloat(x),
		mlScalarToFloat(y),
		mlScalarToFloat(z));
	
	return 0;	// No error.
}

MlRotation
Mle3dCameraCarrier::getOrientation(MleSet *f)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	// Extract the rotation.
	SbRotation r = camera->orientation.getValue();

	float w,x,y,z;
	r.getValue(w,x,y,z);

	// Return the rotation.
	return MlRotation(
		mlFloatToScalar(w),
		mlFloatToScalar(x),
		mlFloatToScalar(y),
		mlFloatToScalar(z));
}

int
Mle3dCameraCarrier::setNearClipping(MleSet *f,MlScalar nearPlane)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	// Set the near clipping.
	camera->nearDistance.setValue(mlScalarToFloat(nearPlane));

	return 0;	// No error.
}

MlScalar
Mle3dCameraCarrier::getNearClipping(MleSet *f)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	return mlFloatToScalar(camera->nearDistance.getValue());
}

int
Mle3dCameraCarrier::setFarClipping(MleSet *f,MlScalar farPlane)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	// Set the far clipping.
	camera->farDistance.setValue(mlScalarToFloat(farPlane));

	return 0;	// No error.
}

MlScalar
Mle3dCameraCarrier::getFarClipping(MleSet *f)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	return mlFloatToScalar(camera->farDistance.getValue());
}

int
Mle3dCameraCarrier::setAspectRatio(MleSet *f,MlScalar aspect)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Aspect ratio handled thru set - zero has special treatment.
	set->setCameraAspect(aspect);

	return 0;	// no error.
}

MlScalar
Mle3dCameraCarrier::getAspectRatio(MleSet *f)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	return mlFloatToScalar(camera->aspectRatio.getValue());
}

int
Mle3dCameraCarrier::setPerspectiveFieldOfView(MleSet *f,MlScalar fov)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	// Get the switch over the cameras.
	SoSwitch *cameraSwitch = set->getCameraSwitch();

	// Set the switch to the perspective camera.
	cameraSwitch->whichChild.setValue(0);

	// Get the perspective camera.
	SoPerspectiveCamera *pcamera = (SoPerspectiveCamera *)set->getCamera();

	// If there has been a change, move state.
	if ( pcamera != camera )
	{
		pcamera->position = camera->position;
		pcamera->orientation = camera->orientation;
		pcamera->nearDistance = camera->nearDistance;
		pcamera->farDistance = camera->farDistance;
		pcamera->aspectRatio = camera->aspectRatio;
	}

	// Now set the field of view.
	pcamera->heightAngle.setValue(mlScalarToFloat(fov));

	return 0;	// No error.
}

MlScalar
Mle3dCameraCarrier::getPerspectiveFieldOfView(MleSet *f)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	// If it is not a perspective camera, return 0.
	if ( camera->isOfType(SoPerspectiveCamera::getClassTypeId()) )
		return mlFloatToScalar(
			((SoPerspectiveCamera *)camera)->heightAngle.getValue());
	else
		return ML_SCALAR(0);
}

int
Mle3dCameraCarrier::setOrthographicViewHeight(MleSet *f,MlScalar height)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	// Get the switch over the cameras.
	SoSwitch *cameraSwitch = set->getCameraSwitch();

	// Set the switch to the orthographic camera.
	cameraSwitch->whichChild.setValue(1);

	// Get the perspective camera.
	SoOrthographicCamera *ocamera = (SoOrthographicCamera *)set->getCamera();

	// If there has been a change, move state.
	if ( ocamera != camera )
	{
		ocamera->position = camera->position;
		ocamera->orientation = camera->orientation;
		ocamera->nearDistance = camera->nearDistance;
		ocamera->farDistance = camera->farDistance;
		ocamera->aspectRatio = camera->aspectRatio;
	}

	// Now set the view height.
	ocamera->height.setValue(mlScalarToFloat(height));

	return 0;	// no error
}

MlScalar
Mle3dCameraCarrier::getOrthographicViewHeight(MleSet *f)
{
	Mle3dSet *set = Mle3dSet::cast(f);

	// Get the Inventor camera.
	SoCamera *camera = set->getCamera();

	// If it is not an orthographic camera, return 0.
	if ( camera->isOfType(SoOrthographicCamera::getClassTypeId()) )
		return mlFloatToScalar(
			((SoOrthographicCamera *)camera)->height.getValue());
	else
		return ML_SCALAR(0);
}

