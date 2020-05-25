/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dlta.cxx
 * @ingroup MleParts
 *
 * This file implements the base class for a Magic Lantern Light.
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
#include "mle/3dlta.h"


MLE_ACTOR_SOURCE(Mle3dBaseLightActor,MleActor);

Mle3dBaseLightActor::Mle3dBaseLightActor()
{ 
    nodeType = Mle3dNodeTypeProperty::LIGHT;
}

#ifdef MLE_REHEARSAL
void Mle3dBaseLightActor::initClass()
{
    // Register parent actor's Members
    mlRegisterActorClass(Mle3dBaseLightActor, MleActor);

    // Register the Property Members
    mlRegisterActorMember(Mle3dBaseLightActor, onOffSwitch, int);
    mlRegisterActorMember(Mle3dBaseLightActor, intensity, MlScalar);
    mlRegisterActorMember(Mle3dBaseLightActor, orientation, MlRotation);
    mlRegisterActorMember(Mle3dBaseLightActor, RG.onOff, int);
    mlRegisterActorMember(Mle3dBaseLightActor, RG.geometry, MlMediaRef);
    mlRegisterActorMember(Mle3dBaseLightActor, position, MlVector3);
    mlRegisterActorMember(Mle3dBaseLightActor, scale, MlScalar);

    //
    // Mark all the properties that belongs to the "transform" property
    // data set.
    //
    mlRegisterActorMemberDataset(Mle3dBaseLightActor, orientation, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle3dBaseLightActor, position, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle3dBaseLightActor, scale, MLE_PROP_DATASET_TRANSFORM);
}

#endif /* MLE_REHEARSAL */

 


