/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file wraptest.cxx
 * @ingroup MleParts
 *
 * This file implements a class for managing spherical constraints.
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


// This package constrains a position vector to remain inside a certain
// sphere.  It schedules itself to automatically run each frame.  This
// package is usually attached to an actor with a Vehicle package and
// manipulates its "position" property.  When the vehicle ventures
// outside of the sphere, it is wrapped around to the antipode, the
// polar-opposite point.  (Note that despite its name, this package can
// constrain any MlVector3 and need not be associated with a Vehicle.)

// Include system header files.
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/wraptest.h"
#include "mle/behavlib.h"
#include "mle/MleDirector.h"
#include "math/rotation.h"

#define DEFAULT_DIAMETER ML_SCALAR(40)


// constructor

VehicleInWrapAroundSphere::VehicleInWrapAroundSphere(MlVector3& p)
    : position(p)
{
    // default sphere parameters
    diameter = DEFAULT_DIAMETER;
    center = MlVector3::zero;

    // schedule actor's update function, save pointer to scheduler item
    g_theTitle->m_theScheduler->insertFunc (PHASE_ACTOR, (MleSchedulerFunc)update, this, this);
}


// destructor

VehicleInWrapAroundSphere::~VehicleInWrapAroundSphere()
{
    g_theTitle->m_theScheduler->remove(this);
}


// This routine constrains a given position vector to remain inside a given
// sphere.  The sphere is defines by a center point and diameter.  When the
// given position ventures outside of the sphere, it is wrapped around to
// the antipode, the polar-opposite point.


void wrapAroundSphereConstraint (const MlVector3 *center,
    const MlScalar diameter, MlVector3 *position)
{
    // relative offset from sphere center to given position
    MlVector3 offset = (*position) - (*center);

    // distance from sphere center to given position
    MlScalar distance = offset.approximateLength();

    // if given position is outside the sphere...
    if (distance > (diameter / 2))
	{
	    // find unit vector away from sphere center
	    MlVector3 wrapAround = offset / distance;

	    // move towards (past) center a distance of "diameter"
	    MlVector3 antipode = offset - (wrapAround * diameter);

	    // adjusted position is sum of antipode and sphere center
	    (*position) = antipode + (*center);
	}
}


// Scheduled update function: constrain the "position" member of the
// object to which the package is attached.


void VehicleInWrapAroundSphere::update(VehicleInWrapAroundSphere *v)
{
    wrapAroundSphereConstraint (&(v->center),
		v->diameter, &(v->position));
}
