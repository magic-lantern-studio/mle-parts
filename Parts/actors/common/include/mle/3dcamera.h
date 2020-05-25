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

#ifndef __MLE_3DCAMERA_H_
#define __MLE_3DCAMERA_H_

#include "mle/mlTypes.h"
#include "mle/MleActor.h"
#include "mle/3dtranlp.h"
#include "mle/3dqrotp.h"
#include "mle/3dcamtp.h"
#include "mle/3dcamclp.h"
#include "mle/3dcamarp.h"
#include "mle/3dcamfvp.h"

class MleSet;

/**
 * @brief This is a 3D camera actor.
 *
 * This is a 3D camera actor.  Its properties encode the basic specification
 * of a perspective or orthographic camera.  These values are applied only
 * on start up; i.e. this is a static camera.
 * 
 * A 3D camera actor needs to be bound to a Mle3dForum.  This is done with
 * a RoleBinding in the workprint that specifies which forum it is
 * attached to.
 * 
 * Currently a RoleBinding is used to specify which forum the camera
 * goes into.  This workprint item also requires the specification of a
 * role type.  The camera actor doesn't use any role, but a legal
 * role for the forum (usually Mle3dRole) must be specified anyway.
 */
class My3dCameraActor : public MleActor
{
	MLE_ACTOR_HEADER(My3dCameraActor);
    
  public:

	My3dCameraActor();

	virtual void init();

	void update(void);

#ifdef MLE_REHEARSAL
	virtual void resolveEdit(const char *property);
#endif
    
	Mle3dCameraTypeProperty         cameraType;
	Mle3dTranslationProperty        position;
	Mle3dQuaternionRotationProperty orientation;

	Mle3dCameraClippingProperty     clipping;
	Mle3dCameraAspectRatioProperty  aspectRatio;
	Mle3dCameraFieldOfViewProperty  fieldOfView;

};


#endif /* __MLE_3DCAMERA_H_ */

