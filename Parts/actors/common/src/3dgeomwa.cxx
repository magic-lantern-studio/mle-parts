/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dgeomwa.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Geometry Actor
 * with Web geometry registry.
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

#include "mle/3dgeomwa.h"


MLE_ACTOR_SOURCE(Mle3dGeometryWebActor,MleActor);


#ifdef MLE_REHEARSAL
void Mle3dGeometryWebActor::initClass()
{
    // Register parent actor's Members
    mlRegisterActorClass(Mle3dGeometryWebActor, MleActor);

    // Register the Property Members
    mlRegisterActorMember(Mle3dGeometryWebActor, renderStyle, int);
    mlRegisterActorMember(Mle3dGeometryWebActor, translation, MlVector3);
    mlRegisterActorMember(Mle3dGeometryWebActor, rotation, MlVector3);
    mlRegisterActorMember(Mle3dGeometryWebActor, scale, MlVector3);
    mlRegisterActorMember(Mle3dGeometryWebActor, geometryRegistry, MlMediaRef);

    //
    // Mark all the properties that belongs to the "transform" property
    // data set.
    //
    mlRegisterActorMemberDataset(Mle3dGeometryWebActor, translation, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle3dGeometryWebActor, rotation, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle3dGeometryWebActor, scale, MLE_PROP_DATASET_TRANSFORM);
}


void
Mle3dGeometryWebActor::resolveEdit(const char* property)
{ 
    if (property)
	{
		if (strcmp("renderStyle", property) == 0)
			  renderStyle.push(this);
		else if (strcmp("translation", property) == 0)
			  translation.push(this);
		else if (strcmp("rotation", property) == 0)
			  rotation.push(this);
		else if (strcmp("scale", property) == 0)
			  scale.push(this);
		else if ((strcmp("geometryRegistry", property) == 0))
		{
			// XXX this is a temporary way to re-apply materials
			geometryRegistry.push(this); 
		}
    } else
	{
		// Update all properties
		update(); 
    }
}
#endif /* MLE_REHEARSAL */

void
Mle3dGeometryWebActor::init()
{
    geometryRegistry.push(this); // XXX
    update();
}


void
Mle3dGeometryWebActor::update()
{
	// XXX This should update all properties
    renderStyle.push(this);
    translation.push(this);
    rotation.push(this);
    scale.push(this);
}


