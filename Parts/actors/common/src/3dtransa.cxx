/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtransa.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a Magic Lantern Transform Actor.
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
#include "mle/3dtransa.h"


MLE_ACTOR_SOURCE(Mle3dTransformActor,MleActor);


Mle3dTransformActor::Mle3dTransformActor()
{ 
}


#ifdef MLE_REHEARSAL
void Mle3dTransformActor::initClass()
{
    // Register parent actor's Members
    mlRegisterActorClass(Mle3dTransformActor, MleActor);

    // Register the Property Members
    mlRegisterActorMember(Mle3dTransformActor, transform, MlTransform);

    //
    // Mark all the properties that belongs to the "transform" property
    // data set.
    //
    mlRegisterActorMemberDataset(Mle3dTransformActor, transform, MLE_PROP_DATASET_TRANSFORM);
}

void Mle3dTransformActor::resolveEdit(const char *)
{
	update();
}
#endif /* MLE_REHEARSAL */


void  Mle3dTransformActor::init()
{ 
    update();
}


void Mle3dTransformActor::update()
{
    transform.push(this);
}

