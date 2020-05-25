/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtrsa.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a Magic Lantern Translation,
 * Rotation, and Scale Actor.
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
#include "mle/3dtrsa.h"


MLE_ACTOR_SOURCE(Mle3dTRSActor,MleActor);


Mle3dTRSActor::Mle3dTRSActor()
{ }


Mle3dTRSActor::~Mle3dTRSActor()
{
	// Unschedule the behave() function
    g_theTitle->m_theScheduler->remove(this);
}


#ifdef MLE_REHEARSAL
void Mle3dTRSActor::initClass()
{
    // Register parent actor's Members
    mlRegisterActorClass(Mle3dTRSActor, MleActor);

    // Register the Property Members
    mlRegisterActorMember(Mle3dTRSActor, nodeType, int);
    mlRegisterActorMember(Mle3dTRSActor, rotation, MlVector3);
    mlRegisterActorMember(Mle3dTRSActor, translation, MlVector3);
    mlRegisterActorMember(Mle3dTRSActor, scale, MlVector3);

    //
    // Mark all the properties that belongs to the "transform" property
    // data set.
    //
    mlRegisterActorMemberDataset(Mle3dTRSActor, rotation, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle3dTRSActor, translation, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle3dTRSActor, scale, MLE_PROP_DATASET_TRANSFORM);
}

void Mle3dTRSActor::resolveEdit(const char *)
{
    update();
}
#endif /* MLE_REHEARSAL */


void  Mle3dTRSActor::init()
{  
    update();

    g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,behave,this,this,1,1);
}


void  Mle3dTRSActor::update()
{
    nodeType.push(this);
    scale.push(this);
    rotation.push(this);
    translation.push(this);
}


void Mle3dTRSActor::behave(void* /*client*/)
{
/*
    Mle3dTRSActor* a = (Mle3dTRSActor*)client;

    if (a)
    { // Do the actor's behavior }
*/
}


