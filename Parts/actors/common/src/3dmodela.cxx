/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmodela.h
 * @ingroup MleParts
 *
 * This file implements the actor for a Magic Lantern Model.
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

#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"
#include "math/3dmath.h"
#include "mle/3dmodela.h"


MLE_ACTOR_SOURCE(Mle3dModelActor,MleActor);


Mle3dModelActor::Mle3dModelActor()
{ 
}

#ifdef MLE_REHEARSAL
void Mle3dModelActor::initClass()
{
  // Register parent actor's Members
  mlRegisterActorClass(Mle3dModelActor, MleActor);

  // Register the Property Members
  mlRegisterActorMember(Mle3dModelActor, position, MlVector3);
  mlRegisterActorMember(Mle3dModelActor, orientation, MlRotation);
  mlRegisterActorMember(Mle3dModelActor, scale, MlScalar);
  mlRegisterActorMember(Mle3dModelActor, model, MediaRef);
  mlRegisterActorMember(Mle3dModelActor, colorMap, MediaRef);
  mlRegisterActorMember(Mle3dModelActor, textureMap, MediaRef);

  //
  // Mark all the properties that belongs to the "transform" property
  // data set.
  //
  mlRegisterActorMemberDataset(Mle3dModelActor, position, MLE_PROP_DATASET_TRANSFORM);
  mlRegisterActorMemberDataset(Mle3dModelActor, orientation, MLE_PROP_DATASET_TRANSFORM);
  mlRegisterActorMemberDataset(Mle3dModelActor, scale, MLE_PROP_DATASET_TRANSFORM);
}


void Mle3dModelActor::resolveEdit(const char* property)
{ 
    if (property)
	{
		if (strcmp("position",property) == 0)
		{
			position.push(this);
		} else if (strcmp("orientation",property) == 0)
		{
			orientation.push(this);
		} else if (strcmp("scale",property) == 0)
		{
			scale.push(this);
		} else if ((strcmp("model",property) == 0) ||
			   (strcmp("colorMap",property) == 0) ||
			   (strcmp("textureMap",property) == 0))
		{
			model.push(this);
			colorMap.push(this);
			textureMap.push(this); 
		}
	} else
	{
		// Update all properties
		update();
    }
}
#endif /* MLE_REHEARSAL */


void  Mle3dModelActor::init()
{
    model.push(this);
    colorMap.push(this);
    textureMap.push(this);
    update();
}


void Mle3dModelActor::update()
{
    position.push(this);
    orientation.push(this);
    scale.push(this);
}

