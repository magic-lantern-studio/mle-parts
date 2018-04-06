/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dpntlta.cxx
 * @ingroup MleParts
 *
 * This file implements the actor for a Magic Lantern Point Light.
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
#include "mle/3dpntlta.h"


MLE_ACTOR_SOURCE(Mle3dPointLightActor,Mle3dBaseLightActor);

Mle3dPointLightActor::Mle3dPointLightActor()
  : Mle3dBaseLightActor()
{ 
    lightType = Mle3dLightTypeProperty::POINT;
}

Mle3dPointLightActor::~Mle3dPointLightActor()
{ 
    onOffSwitch = 0;
    onOffSwitch.push(this);
}

#ifdef MLE_REHEARSAL
void Mle3dPointLightActor::initClass()
{
    // Register parent actor's Members
    mlRegisterActorClass(Mle3dPointLightActor, Mle3dBaseLightActor);
}

void Mle3dPointLightActor::resolveEdit(const char *property)
{ 
    if (property)
	{
		if (strcmp("onOffSwitch",property) == 0) 
		  // if lightswitch has been changed, change the rehearsal geometry
		  // switch accordingly.
		  RG.onOff = onOffSwitch;
	
    }
    update(); 
}
#endif /* MLE_REHEARSAL */


void  Mle3dPointLightActor::init()
{ 
    nodeType.push(this);
    lightType.push(this);
 
    update();
}

void Mle3dPointLightActor::update()
{
    onOffSwitch.push(this);
    intensity.push(this);
    position.push(this);
#ifdef MLE_REHEARSAL
    RG.push(this);
    scale.push(this);
#endif
}
