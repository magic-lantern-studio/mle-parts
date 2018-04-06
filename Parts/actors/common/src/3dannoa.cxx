/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dannoa.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Annotation Actor.
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

#include <stdio.h>

#include "mle/3dannoa.h"
#include "mle/3dannoc.h"
#include "mle/3dpickc.h"

Mle3dAnnotation *Mle3dAnnotation::instance = NULL;

MLE_ACTOR_SOURCE(Mle3dAnnotation,MleActor);

#ifdef MLE_REHEARSAL
void
Mle3dAnnotation::initClass(void)
{
	mlRegisterActorClass(Mle3dAnnotation,MleActor);
}
#endif

Mle3dAnnotation::Mle3dAnnotation(void)
{
#if defined(MLE_DEBUG)
	if ( instance )
	{
		printf("Mle3dAnnotation instance already present.\n");
		return;
	}
#endif
	instance = this;
}

Mle3dAnnotation::~Mle3dAnnotation()
{
	// don't leave a dangling pointer
	instance = NULL;
}

void 
Mle3dAnnotation::init(void)
{
	Mle3dPickableCarrier::set(getRole(),FALSE);
}

void
Mle3dAnnotation::clear(void *context)
{
	// make sure the annotation instance exists
	if ( !instance || !instance->getRole() ) return;

	// call the role function
	Mle3dAnnotationCarrier::clear(instance->getRole(),context);
}

void
Mle3dAnnotation::drawLine(
void *context,
const MlVector3& from,
const MlVector3& to,
float red,
float grn,
float blu)
{
	// make sure the annotation instance exists
	if ( !instance || !instance->getRole() ) return;

	// call the role function
	Mle3dAnnotationCarrier::drawLine(instance->getRole(),context,
		from,to,red,grn,blu);
}

