/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcamc.h
 * @ingroup MleParts
 *
 * This file contains the class for a 3D Camera interface to a Set.
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

#ifndef __MLE_3DCAMERACARRIER_H_
#define __MLE_3DCAMERACARRIER_H_

// Include Magic Lantern Math header files.
#include "math/vector.h"
#include "math/rotation.h"

class MleSet;

/**
 * @brief A camera interface to a Set.
 *
 * This class defines an interface used to modify the camera on a
 * Mle3dSet.  It is not strictly an object, merely a name scoping
 * mechanism for functions.
 */
class Mle3dCameraCarrier
{
  public:

	// Return values for all setting functions are 0 for success,
	//   nonzero for error.  A nonzero return may indicate that
	//   the set does not fully support the desired feature.

	// position

	// setPosition() requests the Set to move the camera to the
	//   indicated point.  Zero is returned if the operation was
	//   successful.
	static int setPosition(MleSet *set,const MlVector3 *position);

	// getPosition() retrieves the current camera position from
	//   the Set.
	static MlVector3 getPosition(MleSet *set);

	// orientation

	// setOrientation() requests the Set to change the viewing
	//   direction of the camera.  The orientation is passed in
	//   as a rotation from the default orientation, which is
	//   pointing in the -Z direction with Y up.  Zero is returned
	//   if the operation is successful.
	static int setOrientation(MleSet *set,const MlRotation *orientation);

	// getOrientation() retrieves the current camera orientation
	//   from the Set.  The orientation is returned as a rotation
	//   from the default orientation, which is pointing in the -Z
	//   direction with Y up.
	static MlRotation getOrientation(MleSet *set);

	// clipping

	// setNearClipping() requests the set to change the location
	//   of the near clipping plane of the camera.  Zero is returned
	//   if the operation is successful.  The near clipping plane
	//   location is a scalar distance from the camera position,
	//   and must be greater than zero and less than the far clipping
	//   plane value.  The allowed ratio of far clipping plane to
	//   near clipping plane will be limited by the z buffer resolution
	//   of the graphics system (if it is a depth buffer implementation).
	static int setNearClipping(MleSet *set,MlScalar clipping);

	// setFarClipping() requests the Set to change the location
	//   of the far clipping plane of the camera.  Zero is returned
	//   if the operation is successful.  The far clipping plane
	//   location is scalar distance from the camera position,
	//   and must be greater than the near clipping plane value.
	//   The allowed ratio of far clipping plane to near clipping
	//   plane will be limited by the z buffer resolution of the
	//   graphics system (if it is a depth buffer implementation).
	static int setFarClipping(MleSet *set,MlScalar clipping);

	// getNearClipping() retrieves the current near clipping plane
	//   distance.
	static MlScalar getNearClipping(MleSet *set);

	// getFarClipping() retrieves the current far clipping plane
	//   distance.
	static MlScalar getFarClipping(MleSet *set);

	// aspect ratio

	// setAspectRatio() requests the Set to change the aspect
	//   ratio of the camera.  Zero is returned if the operation
	//   is successful.
	//
	//   Setting an aspect ratio of zero has a special meaning, which
	//   is to take the aspect from the set shape.
	static int setAspectRatio(MleSet *set,MlScalar aspectRatio);

	// getAspectRatio() retrieves the current camera aspect ratio.
	static MlScalar getAspectRatio(MleSet *set);

	// field of view (for perspective cameras)

	// setPerspectiveFieldOfView() requests the Set to use a
	//   perspective camera and set the field of view.  Field
	//   of view values should be expressed in radians, and
	//   represent the height angle of the camera.  The width
	//   angle will be derived from the height angle and the
	//   aspect ratio.  Zero is returned if the operation is
	//   successful.
	static int setPerspectiveFieldOfView(MleSet *set,MlScalar fov);

	// getPerspectiveFieldOfView() retrieves the current field
	//   of view.  If the Set camera is not a perspective camera
	//   with a field of view, zero is returned.
	static MlScalar getPerspectiveFieldOfView(MleSet *set);

	// height (for orthographic cameras)

	// setOrthographicViewHeight() requests the Set to use a
	//   orthographic camera and set the height of the view.  The
	//   view height is expressed in world coordinate units.  The
	//   view width will be derived from the view height and the
	//   aspect ratio.  Zero is returned if the operation is
	//   successful.
	static int setOrthographicViewHeight(MleSet *set,MlScalar fov);

	// getOrthographicViewHeight() retrieves the current view
	//   height.  If the set camera is not an orthographic camera
	//   with a view height, zero is returned.
	static MlScalar getOrthographicViewHeight(MleSet *set);
};

#endif /* __MLE_3DCAMERACARRIER_H_ */
