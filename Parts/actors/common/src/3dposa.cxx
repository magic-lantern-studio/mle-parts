/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dposa.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Position Orientation and Scale Actor.
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
#include "mle/3dposa.h"


MLE_ACTOR_SOURCE(Mle3dPOSActor,MleActor);


Mle3dPOSActor::Mle3dPOSActor()
{ 
}


Mle3dPOSActor::~Mle3dPOSActor()
{
	// Unschedule the behave() function
    g_theTitle->m_theScheduler->remove(this);
}


#ifdef MLE_REHEARSAL
void Mle3dPOSActor::initClass()
{
    // Register parent actor's Members
    mlRegisterActorClass(Mle3dPOSActor, MleActor);

    // Register the Property Members
    mlRegisterActorMember(Mle3dPOSActor, nodeType, int);
    mlRegisterActorMember(Mle3dPOSActor, position, MlVector3);
    mlRegisterActorMember(Mle3dPOSActor, orientation, MlRotation);
    mlRegisterActorMember(Mle3dPOSActor, scale, MlScalar);

    //
    // Mark all the properties that belongs to the "transform" property
    // data set.
    //
    mlRegisterActorMemberDataset(Mle3dPOSActor, position, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle3dPOSActor, orientation, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle3dPOSActor, scale, MLE_PROP_DATASET_TRANSFORM);
}

void Mle3dPOSActor::resolveEdit(const char *)
{
	update();
}
#endif /* MLE_REHEARSAL */


void  Mle3dPOSActor::init()
{ 
    update();
    g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,behave,this,this,1,1);
}


void Mle3dPOSActor::update()
{
    nodeType.push(this);
    scale.push(this);
    orientation.push(this);
    position.push(this);
}


void Mle3dPOSActor::behave(void* /*client*/)
{
/*
    Mle3dPOSActor* a = (Mle3dPOSActor*)client;

    if (a)
    { // Do the actor's behavior. }
*/
}


