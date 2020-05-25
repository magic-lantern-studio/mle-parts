/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcamc.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for a 3D Camera interface
 * to a Set.
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

// Include Magic Lantern Runtime Engine header files.
#include <mle/MleStage.h>

// Include Magic Lantern Extension header files.
#include <mle/3dcamc.h>

// Include Magic Lantern Math header files.
#include <math/transfrm.h>
#include <math/scalarbr.h>

// Include graphics library specific to BRender.
#include <mle/br3dset.h>
#include <mle/brendapi.h>


int
Mle3dCameraCarrier::setPosition(MleSet *s,const MlVector3 *position)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera.
	br_actor *camera = (br_actor *)set->getCamera();

	// If the current camera is the identity type, change to translation.
	if ( camera->t.type == BR_TRANSFORM_IDENTITY )
		camera->t.type = BR_TRANSFORM_TRANSLATION;
	
	// The following is a minor hack but saves code.
	//   BRender has many different flavors of transforms, but they
	//   all (except identity) save the translation in the last three
	//   words.  This means we can pick any style as long as it isn't
	//   identity and things just work.  This code is vulnerable to
	//   a new addition to transform encodings.
	camera->t.t.translate.t.v[0] = mlScalarToBRScalar((*position)[0]);
	camera->t.t.translate.t.v[1] = mlScalarToBRScalar((*position)[1]);
	camera->t.t.translate.t.v[2] = mlScalarToBRScalar((*position)[2]);

	return 0;	// No error.
}

MlVector3
Mle3dCameraCarrier::getPosition(MleSet *s)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera.
	br_actor *camera = (br_actor *)set->getCamera();

	MlVector3 position;

	// Test for camera transform type.
	if ( camera->t.type == BR_TRANSFORM_IDENTITY )
		position.setValue(ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(0));
	else
	{
		position[0] = mlBRScalarToScalar(camera->t.t.translate.t.v[0]);
		position[1] = mlBRScalarToScalar(camera->t.t.translate.t.v[1]);
		position[2] = mlBRScalarToScalar(camera->t.t.translate.t.v[2]);
	}

	return position;
}

int
Mle3dCameraCarrier::setOrientation(MleSet *s,const MlRotation *orientation)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera.
	br_actor *camera = (br_actor *)set->getCamera();

	// Preserve the current translation.
	//    This is a minor hack, but saves code.  BRender has many
	//    different flavors of transforms, but they all (except
	//    identity) save the translation in the last three words.
	//    This means translation will be preserved if we change
	//    from any translation style except identity.
	if ( camera->t.type == BR_TRANSFORM_IDENTITY )
	{
		camera->t.t.mat.m[3][0] = BR_SCALAR(0);
		camera->t.t.mat.m[3][1] = BR_SCALAR(0);
		camera->t.t.mat.m[3][2] = BR_SCALAR(0);
	}

	camera->t.type = BR_TRANSFORM_MATRIX34;

	// Now set the orientation by quaternion.
	MlTransform t;
	orientation->getValue(t);

	// Set the BRender matrix.
	camera->t.t.mat.m[0][0] = mlScalarToBRScalar(t[0][0]);
	camera->t.t.mat.m[0][1] = mlScalarToBRScalar(t[0][1]);
	camera->t.t.mat.m[0][2] = mlScalarToBRScalar(t[0][2]);
	camera->t.t.mat.m[1][0] = mlScalarToBRScalar(t[1][0]);
	camera->t.t.mat.m[1][1] = mlScalarToBRScalar(t[1][1]);
	camera->t.t.mat.m[1][2] = mlScalarToBRScalar(t[1][2]);
	camera->t.t.mat.m[2][0] = mlScalarToBRScalar(t[2][0]);
	camera->t.t.mat.m[2][1] = mlScalarToBRScalar(t[2][1]);
	camera->t.t.mat.m[2][2] = mlScalarToBRScalar(t[2][2]);

	return 0;	// No error.
}


MlRotation
Mle3dCameraCarrier::getOrientation(MleSet *s)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Set the BRender camera.
	br_actor *camera = (br_actor *)set->getCamera();

	MlRotation orientation;		// Return value.

	// Convert the transform to a matrix
	br_matrix34 matrix;
	BrTransformToMatrix34(&matrix,&camera->t);

	// Convert the BRender matrix to a MlTransform.
	MlTransform r;
	r[0][0] = mlBRScalarToScalar(matrix.m[0][0]);
	r[0][1] = mlBRScalarToScalar(matrix.m[0][1]);
	r[0][2] = mlBRScalarToScalar(matrix.m[0][2]);
	r[1][0] = mlBRScalarToScalar(matrix.m[1][0]);
	r[1][1] = mlBRScalarToScalar(matrix.m[1][1]);
	r[1][2] = mlBRScalarToScalar(matrix.m[1][2]);
	r[2][0] = mlBRScalarToScalar(matrix.m[2][0]);
	r[2][1] = mlBRScalarToScalar(matrix.m[2][1]);
	r[2][2] = mlBRScalarToScalar(matrix.m[2][2]);
	r[3][0] = r[3][1] = r[3][2] = ML_SCALAR(0);

	// Initialize the rotation with the transform.
	orientation.setValue(r);

	return orientation;
}


int
Mle3dCameraCarrier::setNearClipping(MleSet *s,MlScalar nearPlane)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera
	br_actor *camera = (br_actor *)set->getCamera();

	// XXX BRender has a bug with calculating the near clipping
	//   plane.  If the hither and yon planes are desired to
	//   be at nearPlane and farPlane, then you have to tell
	//   BRender to put the near clipping plane at
	//   (farPlane*nearPlane)/(2*farPlane - nearPlane)

	MlScalar farPlane = mlBRScalarToScalar(
		((br_camera *)camera->type_data)->yon_z);
	((br_camera *)camera->type_data)->hither_z = mlScalarToBRScalar(
		mlMulDiv(farPlane,nearPlane,farPlane + farPlane - nearPlane));
	
	return 0;	// No error.
}

MlScalar
Mle3dCameraCarrier::getNearClipping(MleSet *s)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera
	br_actor *camera = (br_actor *)set->getCamera();

	// XXX BRender has a bug with calculating perspective
	//   transformations.  You get the wrong near clipping
	//   plane.  If BRender thinks the planes are at near
	//   and far, the actual near is
	//   2*farPlane*nearPlane/(farPlane + nearPlane)
	MlScalar nearPlane = mlBRScalarToScalar(
		((br_camera *)camera->type_data)->hither_z);
	MlScalar farPlane = mlBRScalarToScalar(
		((br_camera *)camera->type_data)->yon_z);
	
	return mlMulDiv(nearPlane + nearPlane,farPlane,farPlane + nearPlane);
}

int
Mle3dCameraCarrier::setFarClipping(MleSet *s,MlScalar farPlane)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera.
	br_actor *camera = (br_actor *)set->getCamera();

	((br_camera *)camera->type_data)->yon_z = mlScalarToBRScalar(farPlane);

	return 0;	// No error.
}

MlScalar
Mle3dCameraCarrier::getFarClipping(MleSet *s)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera.
	br_actor *camera = (br_actor *)set->getCamera();

	return mlBRScalarToScalar(
		((br_camera *)camera->type_data)->yon_z);
}

int
Mle3dCameraCarrier::setAspectRatio(MleSet *s,MlScalar aspect)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera.
	br_actor *camera = (br_actor *)set->getCamera();

	// Special case zero to mean set with the stage aspect ratio.
	if ( aspect == ML_SCALAR_ZERO )
	{
		// Get the stage size.
		int width,height;
#ifdef MLE_REHEARSAL
		MleStage::g_theStage->getSize(&width,&height);
#else
		// XXX quick fix for build.
		width = 640;
		height = 480;
#endif
		
		// Overwrite the aspect.
		aspect = mlDiv(mlLongToScalar(width),mlLongToScalar(height));
	}

	((br_camera *)camera->type_data)->aspect = mlScalarToBRScalar(aspect);

	return 0;	// No error.
}

MlScalar
Mle3dCameraCarrier::getAspectRatio(MleSet *s)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera
	br_actor *camera = (br_actor *)set->getCamera();

	return mlBRScalarToScalar(
		((br_camera *)camera->type_data)->aspect);
}

int
Mle3dCameraCarrier::setPerspectiveFieldOfView(MleSet *s,MlScalar fov)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera.
	br_actor *camera = (br_actor *)set->getCamera();

	// Make this a perspective camera.
	((br_camera *)camera->type_data)->type = BR_CAMERA_PERSPECTIVE;

	// Set the field of view.
	((br_camera *)camera->type_data)->field_of_view =
		BrRadianToAngle(mlScalarToBRScalar(fov));
	
	return 0;	// No error.
}

MlScalar
Mle3dCameraCarrier::getPerspectiveFieldOfView(MleSet *s)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera.
	br_actor *camera = (br_actor *)set->getCamera();

	// return 0 if this is not a perspective camera
	//   Amazingly enough, this is a physically valid return for
	//   a parallel projection.
	if ( ((br_camera *)camera->type_data)->type == BR_CAMERA_PERSPECTIVE )
		return mlBRScalarToScalar(BrAngleToScalar(((br_camera *)camera->type_data)->field_of_view));
	else
		return ML_SCALAR(0);
}

int
Mle3dCameraCarrier::setOrthographicViewHeight(MleSet *s,MlScalar height)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera.
	br_actor *camera = (br_actor *)set->getCamera();

	// Make this an orthographic camera.
	((br_camera *)camera->type_data)->type = BR_CAMERA_PARALLEL;

	// Set the height.
	//   And set the width using the aspect ratio.
	((br_camera *)camera->type_data)->height = mlScalarToBRScalar(height);
	((br_camera *)camera->type_data)->width =
		BR_MUL(((br_camera *)camera->type_data)->height,
			((br_camera *)camera->type_data)->aspect);

	return 0;	// no error;
}

MlScalar
Mle3dCameraCarrier::getOrthographicViewHeight(MleSet *s)
{
	Mle3dSet *set = Mle3dSet::cast(s);

	// Get the BRender camera.
	br_actor *camera = (br_actor *)set->getCamera();

	// Return 0 if this is not an orthographic camera
	if ( ((br_camera *)camera->type_data)->type == BR_CAMERA_PARALLEL )
		return mlBRScalarToScalar(
			((br_camera *)camera->type_data)->height);
	else
		return ML_SCALAR(0);
}

