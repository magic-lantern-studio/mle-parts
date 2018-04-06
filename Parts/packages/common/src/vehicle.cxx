/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file vehicle.h
 * @ingroup MleParts
 *
 * This file defines a class for a Vehicle.
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
#include "mle/vehicle.h"

#include "math/scalar.h"
#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"

#include "mle/behavlib.h"


Vehicle::Vehicle (MlVector3& p,
		  MlRotation& o,
		  MlScalar& s,
		  Reorient& mybanker,
		  Stabilizer& mystabilizer, 
		  MlBoolean schedFlag)
 : SteerableMass(p,o,FALSE),
   scale(s),
   banker(mybanker),
   stabilizer(mystabilizer)
{
	// set up default state
	maxSpeed            = DEFAULT_VEHICLE_MAX_SPEED;
	maxForce            = DEFAULT_VEHICLE_MAX_FORCE;
	radius              = DEFAULT_VEHICLE_RADIUS;
	collisionThreatGain = DEFAULT_VEHICLE_THREAT;
	previousScale       = ML_SCALAR_ONE;

	// add the position update to the scheduler
	if ( schedFlag )
		g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,
			(MleSchedulerFunc)update,this,this);
}

Vehicle::~Vehicle()
{
    g_theTitle->m_theScheduler->remove(this);
}

// Given a "proposed steering force", condition it so that the point-mass-
// based physical model will move in a vehicle-like fashion.  These input
// vectors ("proposed forces") are typically based on distances and so are
// much longer than realistic force vectors.  The truncateVectorLength call
// handles this.  The call to adjustSteeringForVelocity gives the point
// mass a tendency to resist abrupt changes in orientation.  Finally the
// adjusted force is applied to the point mass in local or global space.


#define MODULATE_PROPOSED_STEERING_FORCE(applyForce)     \
{                                                        \
    MlVector3 force = steering;                          \
    truncateVectorLength (&force, maxForce);             \
    adjustSteeringForVelocity (this, maxSpeed, &force);  \
    applyForce (force);                                  \
}


void Vehicle::applyLocalSteeringForce (const MlVector3& steering)
{
    MODULATE_PROPOSED_STEERING_FORCE (applyLocalForce);
}


void Vehicle::applyGlobalSteeringForce (const MlVector3& steering)
{
   MlVector3 localSteering;

   localizeVector(steering,localSteering);

   truncateVectorLength(&localSteering,maxForce);
   adjustSteeringForVelocity(this,maxSpeed,&localSteering);
   applyLocalForce(localSteering);
}


MlScalar Vehicle::collisionThreatDistance (void) const
{
    // This is an ad hoc predictor that defines the "collision danger
    // threshold" as directly proportional to speed and inversely
    // proportional to agility.
    MlScalar approxSpeed = velocity.approximateLength();
    return mlMul (collisionThreatGain, mlDiv (approxSpeed, maxForce));
}


void Vehicle::handleScaleChange (void)
{
    if (scale != previousScale)
	{
	    MlScalar change = mlDiv (scale, previousScale);
	    radius = mlMul (radius, change);
	    // mass = mlMul (mass, mlMul (change, mlMul (change, change)));
	    previousScale = scale;
	}
}


void
Vehicle::update(Vehicle *sv)
{
	// limit velocity and acceleration
        
    if (sv->maxSpeed != ML_SCALAR_ZERO)
	    truncateVectorLength(&sv->velocity,sv->maxSpeed);

	truncateVectorLength(&sv->acceleration,sv->maxForce);

	// monitor, and adjust for, changes in actor scale
	sv->handleScaleChange();

	// stabilize by adjusting forces at low velocity
	sv->stabilizer.update(&sv->stabilizer);

	// update the physics
	SimpleMass::update((SimpleMass *)sv);

	// update the orientation
	sv->banker.adjustVehicle();

	// set acceleration to zero
	sv->acceleration = MlVector3::zero;
}
