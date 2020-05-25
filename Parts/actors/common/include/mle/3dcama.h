/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcama.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Camera Actor.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2019 Wizzer Works
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

#ifndef __MLE_3DCAMA_H_
#define __MLE_3DCAMA_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleActor.h"
#include "mle/Mle3dCameraActor.h"

// Inlclude Magic Lantern Math header files.
#include "math/vector.h"
#include "math/rotation.h"

class MleSet;

/**
 * @brief This is a 3D camera actor.
 *
 * This is a 3D camera actor.  Its properties encode the basic specification
 * of a perspective or orthographic camera.  These values are applied only
 * on start up; i.e. this is a static camera.
 *
 * If the camera is to be moved during title execution, there is an API
 * to set each of the parameters.  This API should be used instead of the
 * properties during runtime so that the new values will be implemented
 * in the Set.
 *
 * A 3D camera actor needs to be bound to a Mle3dSet.  This is done with
 * a RoleBinding in the workprint that specifies which Set it is
 * attached to.
 *
 * BUGS
 * Currently a RoleBinding is used to specify which Set the camera
 * goes into.  This workprint item also requires the specification of a
 * role type.  The camera actor doesn't use any role, but a legal
 * role for the Set (usually Mle3dRole) must be specified anyway.
 */
class MLE_3DCAMERAACTOR_API Mle3dCameraActor : public MleActor
{
	MLE_ACTOR_HEADER(Mle3dCameraActor);
    
  public:

    static void getProperty(MleObject *object, const char *name, unsigned char **value); // value is an output parameter.
    static void setProperty(MleObject *object, const char *name, unsigned char *value); // value is an input parameter.

    // Property declarations.
    MLE_ACTOR_PROPERTY(MlVector3, position, getPositionProperty, setPositionProperty)
    MLE_ACTOR_PROPERTY(MlRotation, orientation, getOrientationProperty, setOrientationProperty)
    MLE_ACTOR_PROPERTY(MlScalar, nearClipping, getNearClippingProperty, setNearClippingProperty)
	MLE_ACTOR_PROPERTY(MlScalar, farClipping, getFarClippingProperty, setFarClippingProperty)
	MLE_ACTOR_PROPERTY(MlScalar, aspectRatio, getAspectRatioProperty, setAspectRatioProperty)
	MLE_ACTOR_PROPERTY(int, cameraType, getCameraTypeProperty, setCameraTypeProperty)
	MLE_ACTOR_PROPERTY(MlScalar, viewHeight, getViewHeightProperty, setViewHeightProperty)

	Mle3dCameraActor();

	virtual void init();

#ifdef MLE_REHEARSAL
	virtual void resolveEdit(const char *property);
#endif /* MLE_REHEARSAL */

#if 0
	// Property declarations.

	// This is the 3D position of the camera.
	MlVector3 position;

	// This is the direction that the camera points in.
	MlRotation orientation;

	// This specifies the distance from the camera position to the near
	//   clipping plane.  Any geometry in front of this plane will not be
	//   drawn.  The ratio between the far clipping distance and the near
	//   clipping distance by the dynamic range of the depth buffer.
	MlScalar nearClipping;

    // This specifies the distance from the camera position to the far
    //   clipping plane.  Any geometry behind this plane will not be
    //   drawn.  The ratio between the far clipping distance and the near
    //   clipping distance by the dynamic range of the depth buffer.
	MlScalar farClipping;

	// This specifies the relative scaling of the horizontal and vertical
    //   axes of the viewport, controlling (for example) whether circles
    //   are drawn as true circles or as ovals.  A value of zero (the
    //   default) is a special cue to make the camera aspect the same as
    //   the stage aspect.
	MlScalar aspectRatio;

    // This specifies what type of camera is to be used.  0 is for a
    //   perspective camera (the default) and 1 is for an orthographic
    //   camera.  An orthographic camera displays objects at the same
    //   screen size regardless of their distance from the camera position.
	int cameraType;

	// This specifies the field of view (in radians) for a perspective
    //   camera and a screen height (in world coordinates) for an
	//   orthographic camera.
	MlScalar viewHeight;
#endif

	// Public API to set and get camera parameters.

	void setPosition(const MlVector3 *position,MlBoolean pushToForum=TRUE);

	void getPosition(MlVector3 *position);

	void setOrientation(const MlRotation *orientation,MlBoolean pushToForum=TRUE);

	void getOrientation(MlRotation *orientation);

	void setNearClipping(MlScalar nearClipping,MlBoolean pushToForum=TRUE);

	void setFarClipping(MlScalar farClipping,MlBoolean pushToForum=TRUE);

	MlScalar getNearClipping(void);

	MlScalar getFarClipping(void);

	void setAspectRatio(MlScalar aspectRatio,MlBoolean pushToForum=TRUE);

	MlScalar getAspectRatio(void);

	void setCameraType(int cameraType,MlBoolean pushToForum=TRUE);

	int getCameraType(void);

	void setViewHeight(MlScalar viewHeight,MlBoolean pushToForum=TRUE);

	MlScalar getViewHeight(void);

	/**
	 * Push all camera state to the Set.
	 */
	void update(void);

  protected:

	/** Remember the owning Set. */
	MleSet *m_set; 
};


#endif /* __MLE_3DCAMA_H_ */
