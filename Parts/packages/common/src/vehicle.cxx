/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file vehicle.h
 * @ingroup MleParts
 *
 * This file defines a class for a Vehicle.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2024 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this source file, contact Mark S. Millard,
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
