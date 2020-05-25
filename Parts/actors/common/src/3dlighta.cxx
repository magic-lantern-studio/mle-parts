/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dlighta.cxx
 * @ingroup MleParts
 *
 * This file implements the actor for a Magic Lantern Light.
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
#include "mle/3dlighta.h"


MLE_ACTOR_SOURCE(Mle3dLightActor,MleActor);

Mle3dLightActor::Mle3dLightActor()
{ 
  nodeType = Mle3dNodeTypeProperty::LIGHT;
  lightType = Mle3dLightTypeProperty::DIRECTIONAL;
}

Mle3dLightActor::~Mle3dLightActor()
{
    onOffSwitch = 0;
    onOffSwitch.push(this);
}

#ifdef MLE_REHEARSAL
void Mle3dLightActor::initClass()
{
    // Register parent actor's Members
    mlRegisterActorClass(Mle3dLightActor, MleActor);

    // Register the Property Members
    mlRegisterActorMember(Mle3dLightActor, position, MlVector3);
    mlRegisterActorMember(Mle3dLightActor, orientation, MlRotation);
    mlRegisterActorMember(Mle3dLightActor, lightType, int);
    mlRegisterActorMember(Mle3dLightActor, onOffSwitch, int);
    mlRegisterActorMember(Mle3dLightActor, intensity, MlScalar);

    //
    // Mark all the properties that belongs to the "transform" property
    // data set.
    //
    mlRegisterActorMemberDataset(Mle3dLightActor, position, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle3dLightActor, orientation, MLE_PROP_DATASET_TRANSFORM);
}


void Mle3dLightActor::resolveEdit(const char *property)
{ 
    if (property)
	{
		if (! strcmp("position",property)) 
		   position.push(this);
		if (! strcmp("orientation",property)) 
		   orientation.push(this);
		if (! strcmp("lightType",property)) 
		   lightType.push(this);
		if (! strcmp("onOffSwitch",property)) 
		   onOffSwitch.push(this);
		if (! strcmp("intensity",property)) 
		   intensity.push(this);
    }
    update(); 
}
#endif /* MLE_REHEARSAL */


void  Mle3dLightActor::init()
{ 
    update();
}


void Mle3dLightActor::update()
{
    nodeType.push(this);
    position.push(this);
    orientation.push(this);
    lightType.push(this);
    onOffSwitch.push(this);
    intensity.push(this);
}

