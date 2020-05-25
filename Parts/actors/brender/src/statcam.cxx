/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file statcam.h
 * @ingroup MleParts
 *
 * This file defines the class for a Static Camera Actor
 * for the BRender platform.
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

// Include system header files.
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlAssert.h"
#include "mle/MleDirector.h"
#include "mle/MleScheduler.h"
#include "mle/MleSet.h"
#include "math/scalar.h"
#include "math/vector.h"
#include "mle/br3dset.h"
#include "mle/statcam.h"


MLE_ACTOR_SOURCE(StaticCamera,MleActor);

#ifdef MLE_REHEARSAL
void
StaticCamera::initClass(void)
{
	mlRegisterActorClass(StaticCamera,MleActor);

	mlRegisterActorMember(StaticCamera,position,FwVector3);
	mlRegisterActorMember(StaticCamera,orientation,FwRotation);
	mlRegisterActorMember(StaticCamera,nearClipping,FwScalar);
	mlRegisterActorMember(StaticCamera,farClipping,FwScalar);

	//
	// Mark all the properties that belongs to the "transform" property
	// data set.
	//
	mlRegisterActorMemberDataset(StaticCamera,position,MLE_PROP_DATASET_TRANSFORM);
	mlRegisterActorMemberDataset(StaticCamera,orientation,MLE_PROP_DATASET_TRANSFORM);
}
#endif /* MLE_REHEARSAL */

StaticCamera::StaticCamera(void)
{
	// Schedule our function.
//	g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,(MleSchedulerFunc)update,this);

	// Initialize properties.
	position.setValue(ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(3));
	orientation.setValue(ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(1));
	nearClipping = ML_SCALAR(0.1);
	farClipping = ML_SCALAR(100);

    // Already being done in init(), plus need Set which doesn't
	// get set until init
//	Mle3dSet::cast(m_set)->setCameraPosition(&position);
//	Mle3dSet::cast(m_set)->setCameraOrientation(&orientation);
}

void
StaticCamera::init(void)
{
    // Moved from constructor, because 
    //   MleSet::currentSet isn't set until init() is called.
    // Remember the Set.
    m_set = Mle3dSet::cast(MleSet::currentSet);

    Mle3dSet::cast(m_set)->setCameraPosition(&position);
    Mle3dSet::cast(m_set)->setCameraOrientation(&orientation);

    // note that far clipping must be set before near clipping because
    //   of a BRender bug
    Mle3dSet::cast(m_set)->setCameraFarClipping(farClipping);
    Mle3dSet::cast(m_set)->setCameraNearClipping(nearClipping);
}

#ifdef MLE_REHEARSAL
void
StaticCamera::resolveEdit(const char *)
{
    Mle3dSet::cast(m_set)->setCameraPosition(&position);
    Mle3dSet::cast(m_set)->setCameraOrientation(&orientation);
    Mle3dSet::cast(m_set)->setCameraNearClipping(nearClipping);
    Mle3dSet::cast(m_set)->setCameraFarClipping(farClipping);
}
#endif /* MLE_REHEARSAL */

#include <stdlib.h>

// XXX -- this routine isn't being used yet

void
StaticCamera::update(StaticCamera *mycamera)
{
	MLE_ASSERT(mycamera->m_set);

	MlVector3 position;

    // XXX -- Use rand() instead of random() because PC doesn't
    //        support it. Also need to seed it somewhere (srand()).
	position[0] = mlFloatToScalar((rand() % 256 - 128)/128.0);
	position[1] = mlFloatToScalar((rand() % 256 - 128)/128.0);
	position[2] = mlFloatToScalar((rand() % 256 - 128)/128.0 + 3);

	Mle3dSet::cast(mycamera->set)->setCameraPosition(&position);
}

