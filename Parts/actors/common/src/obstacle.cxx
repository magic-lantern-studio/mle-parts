/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file obstacle.cxx
 * @ingroup MleParts
 *
 * This file implements the class for an Obstacle Actor.
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
#include "math/scalar.h"
#include "mle/obstacle.h"


MLE_ACTOR_SOURCE(MleObstacle,MleActor);


// Note that the actor's "scale" property and the SphericalMleObstacle
// package's "size" member are being linked up.  This keeps the obstacle's
// diameter in sync with the actor's displayed size when adjusted with the
// SceneEditor's manipulator.


MleObstacle::MleObstacle() : so(position.m_translation,scale.m_scale)
{ 
    // Initial obstacle scale
    scale = ML_SCALAR(5);
}


MleObstacle::~MleObstacle(void)
{
}


#ifdef MLE_REHEARSAL
void MleObstacle::initClass()
{
    // Register parent actor's Members
    mlRegisterActorClass(MleObstacle, MleActor);

    // Register the Property Members
    mlRegisterActorMember(MleObstacle, position, MlVector3);
    mlRegisterActorMember(MleObstacle, orientation, MlRotation);
    mlRegisterActorMember(MleObstacle, model, MlMediaRef);
    mlRegisterActorMember(MleObstacle, colorMap, MlMediaRef);
    mlRegisterActorMember(MleObstacle, textureMap, MlMediaRef);
    mlRegisterActorMember(MleObstacle, scale, MlScalar);

    //
    // Mark all the properties that belongs to the "transform" property
    // data set.
    //
    mlRegisterActorMemberDataset(MleObstacle, position, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(MleObstacle, orientation, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(MleObstacle, scale, MLE_PROP_DATASET_TRANSFORM);
}


void MleObstacle::resolveEdit(const char *)
{
	update();
}
#endif /* MLE_REHEARSAL */


void  MleObstacle::init()
{
    update();
}


void MleObstacle::update()
{
    position.push(this);
    orientation.push(this);
    model.push(this);
    colorMap.push(this);
    textureMap.push(this);
    scale.push(this);
}
