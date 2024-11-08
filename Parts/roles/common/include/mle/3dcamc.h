/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcamc.h
 * @ingroup MleParts
 *
 * This file contains the class for a 3D Camera interface to a Set.
 */

 // COPYRIGHT_BEGIN
 //
 // The MIT License (MIT)
 //
 // Copyright (c) 2000-2024 Wizzer Works
 //
 // Permission is hereby granted, free of charge, to any person obtaining a copy
 // of this software and associated documentation files (the "Software"), to deal
 // in the Software without restriction, including without limitation the rights
 // to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 // copies of the Software, and to permit persons to whom the Software is
 // furnished to do so, subject to the following conditions:
 //
 // The above copyright notice and this permission notice shall be included in all
 // copies or substantial portions of the Software.
 //
 // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 // SOFTWARE.
 //
 //  For information concerning this header file, contact Mark S. Millard,
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

#include "mle/MleCarrier.h"

class MleSet;

/**
 * @brief A camera interface to a Set.
 *
 * This class defines an interface used to modify the camera on a
 * Mle3dSet.  It is not strictly an object, merely a name scoping
 * mechanism for functions.
 */
class MLE_CARRIER_API Mle3dCameraCarrier
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
