/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file behavlib.cxx
 * @ingroup MleParts
 *
 * This file implements utilities for the Magic Lantern
 * behavior library.
 * <p>
 * This is a library of support and utility functions shared by
 * various parts of the Magic Lantern behavioral system.  Some of these
 * functions may have application elsewhere and should be moved to
 * other libraries.  In no sense is this intended to be complete, it
 * is just a collection of utilties created to support existing
 * behavioral routines.  Current contents include:
 * <li>
 * <ul>Math and Geometry utilities</ul>
 * <ul>Time Library</ul>
 * <ul>Vehicle Library</ul>
 * </li>
 * </p>
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
#include <stdlib.h>

// Include Magic Lantern header files.
#include <math/scalar.h>
#include "mle/behavlib.h"
#include "mle/steermas.h"


// Math and Geometry utilities

// Returns a random number between the two specified bounds

MlScalar frand2 (MlScalar min, MlScalar max)
{
    return (min + (mlMul(FRAND01, (max - min))));
}


MlBoolean pointsWithinDistance (MlScalar distance,
    const MlVector3 *v1, const MlVector3 *v2)
{
    MlVector3 difference = (*v1) - (*v2);

    return (VECTOR_WITHIN_CUBE (distance, difference) &&
	    VECTOR_WITHIN_MAGNITUDE (distance, difference));
}


// If the length of the given vector excedes the specified maximum length,
// truncate the length of the vector to that length, otherwise do nothing.

void truncateVectorLength (MlVector3 *vector, const MlScalar maxLength)
{
#if ML_FIXED_POINT
	MlScalar len = (*vector).length();
	if (len > maxLength)
	{
		(*vector) *= mlDiv( maxLength , len );
	}
#else
    MlScalar magnitudeSquared = (*vector).dot(*vector);
    if (magnitudeSquared > mlMul(maxLength, maxLength))
	{
	    (*vector) *= mlDiv( maxLength , mlSqrt(magnitudeSquared) );
	}
#endif
}


// set a given vector to a random point somewhere within a cube of a given
// "radius" (half edge length).

void setToRandomPointInCube (MlVector3 *vector, MlScalar radius)
{
    vector->setValue (frand2 (-radius, radius), 
	    frand2 (-radius, radius), frand2 (-radius, radius));
}


// Time Library (hooks for variable frame rate)

#define DEFAULT_FIXED_FRAME_RATE ML_SCALAR(30)


MlScalar frameRate (void)
{
    return (DEFAULT_FIXED_FRAME_RATE);
}


MlScalar frameDuration (void)
{
    return (mlReciprocal(DEFAULT_FIXED_FRAME_RATE));
}


// Vehicle Library  (XXX - should this be a separate file?)


// utility to get a vehicle's speed

MlScalar vehicleSpeed (const SteerableMass *vehicle)
{
    return ((*vehicle).velocity.length());
}


// Given a vehicle and a target position expressed in the vehicle's local
// coordinate system, determine a steering direction.  The magnitude of
// this vector will be large, and is related to the target distance.

// Note that if the target is directly behind the vehicle it will slow to a
// stop and snap around.  This could be fixed here (by forcing the returned
// value to be more lateral) or in the vehicle (by using moment of inertia
// or orientation damping).

// This STEERING_STRENGTH factor controls the lateral emphasis used
// for steering toward off-axis targets.  Perhaps this should be a
// registered member variable of the vehicle (mass?) class?


#define STEERING_STRENGTH ML_SCALAR(3)


void steerTowardLocalTarget (const SteerableMass *vehicle,
    const MlVector3 *localTarget, MlVector3 *steering)
{
    // Local components of target
    MlScalar forward = (*vehicle).getForward(*localTarget);
    MlScalar right = (*vehicle).getRight(*localTarget);
    MlScalar up = (*vehicle).getUp(*localTarget);
    // Symmetric version of lateral components
    MlScalar absRight =  mlAbs (right);
    MlScalar absUp = mlAbs (up);
    // Larger of the two
    MlScalar maxLateral = MLE_MAX (absRight, absUp);
    // Is the target within a forward-facing 45 degree pyramid 
    MlBoolean ahead = forward > maxLateral;

    // Start with "naive pursuit" (steer towards target)
    *steering = *localTarget;

    // If not heading generally towards the target, put on the brakes:
    // set the forward steering component to be strongly negative.
    if (!ahead)
	{
	    MlScalar newForward = (maxLateral == ML_SCALAR_ZERO) ? -ML_SCALAR_ONE : -maxLateral;
	    (*vehicle).setForward(*steering, newForward);
	}

    // Scale up in the direction purpendicular to the forward axis
    (*vehicle).setRight(*steering, mlMul(right, STEERING_STRENGTH));
    (*vehicle).setUp   (*steering, mlMul(up, STEERING_STRENGTH));
}


// Given a vehicle and a target position expressed in global space,
// determine a steering direction.  The magnitude of this vector will be
// large, and is related to the target distance.

void steerTowardGlobalTarget (const SteerableMass *vehicle,
			      const MlVector3 *globalTarget,
			      MlVector3 *steering)
{
    MlVector3 localTarget;

    // Transform global target point into local space
    (*vehicle).localizeCoordinate(*globalTarget, localTarget);

    // Determine steering
    steerTowardLocalTarget (vehicle, &localTarget, steering);
}


// Inertial Damping hack:

// Given a vehicle and a proposed steering force, adjust the force as if
// limited by rotational inertia, and/or by a vehicle where turning is
// dependent on speed (eg a automobile: must be moving in order to turn)

// maxSpeed should probably be part of the SteerableMass (or a new speed-
// damped vehicle class), and so obtained from the vehicle pointer rather
// than being pass in from the caller.

// adjustSteeringForVelocity works by attenuating the lateral components of
// the proposed steering force at low vehicle speeds.  The meaning of "low
// speed" is parameterized by ADJUST_THRESHOLD.  It is a number between 0
// and 1, representing zero speed and maximum speed respectively.  Vehicle
// speeds between 0 and ADJUST_THRESHOLD trigger the adjustment of steering
// forces.


#define ADJUST_THRESHOLD ML_SCALAR(0.3f)


void adjustSteeringForVelocity (const SteerableMass *vehicle,
				const MlScalar maxSpeed,
				MlVector3 *steering)
{
    // If our speed is below a certain fraction of our maximum speed...
    MlScalar currentSpeed = vehicleSpeed (vehicle);
    MlScalar relativeSpeed = mlDiv(currentSpeed, maxSpeed);
    if (relativeSpeed < ADJUST_THRESHOLD)
	{
	    // Extract components of local steering
	    MlScalar forward = (*vehicle).getForward(*steering);
	    MlScalar right   = (*vehicle).getRight  (*steering);
	    MlScalar up      = (*vehicle).getUp     (*steering);

	    // If proposed steering force has any lateral component...
	    if ((right != ML_SCALAR_ZERO) || (up != ML_SCALAR_ZERO))
		{
		    // Weight by speed relative to threshold...
		    MlScalar weight = mlDiv(relativeSpeed, ADJUST_THRESHOLD);
		    // ...squared.
		    weight = mlSquare(weight);

		    MlScalar af = mlAbs(forward);
		    MlScalar ar = mlAbs(right);
		    MlScalar au = mlAbs(up);
		    MlScalar newForward = MLE_MAX (af, ar);
		    // newForward is maximum of steering magnitudes 
		    newForward = MLE_MAX (newForward, au);
		    // Blend weighted and unweighted forwards:
		    newForward = ML_LERP (ML_SCALAR(0.9f), newForward,
					  mlMul(newForward, weight));

		    // Rebuild steering force from adjusted components
		    (*vehicle).setForward(*steering, newForward);
		    (*vehicle).setRight  (*steering, mlMul(weight, right));
		    (*vehicle).setUp     (*steering, mlMul(weight, up));
		}

	    // Prevent orientation flip to to excess braking force
	    else
		{
		    // If this is "pure braking" force (straight backward)
		    if (forward < ML_SCALAR_ZERO)
			{
			    // Limit deceleration to magnitude of velocity
			    MlScalar mass = (*vehicle).mass;
			    MlScalar dSpeed = mlDiv( forward, mass );
			    
			    if ((currentSpeed > ML_SCALAR_ZERO) &&
				    ((-dSpeed) > currentSpeed))
				{
				    MlScalar brake = -mlMul(currentSpeed, mass);
				    (*vehicle).setForward(*steering, brake);
				}
			    
			}
		}

	    // Prevent flip-flopping from too much reverse thrust when
	    // Speed is nearly zero
	    if ( (relativeSpeed < mlDiv(ADJUST_THRESHOLD, ML_SCALAR(6))) &&
		     (forward < ML_SCALAR_ZERO) )
	      *steering = MlVector3::zero;
	}
}


// XXX temp hack
// stripped this code out of steerTowardLocalTarget

// Given a vehicle and a target point in the vehicle's local coordinate
// space, determine if the target is near the vehicle's "forward"
// direction.  That is, if the target is within a pyramid whose apex is at
// the vehicle's and whose central axis extends along the vehicle's forward
// direction.


MlBoolean targetNearForwardAxis (const SteerableMass *vehicle,
				 const MlVector3 *localTarget)
{
    // Local components of target
    MlScalar forward = (*vehicle).getForward(*localTarget);
    MlScalar right = (*vehicle).getRight(*localTarget);
    MlScalar up = (*vehicle).getUp(*localTarget);
    // Symmetric version of lateral components
    MlScalar absRight =  mlAbs (right);
    MlScalar absUp = mlAbs (up);
    // Larger of the two
    MlScalar maxLateral = MLE_MAX (absRight, absUp);
    // Is the target within a forward-facing 45 degree pyramid 
    MlBoolean ahead = forward > maxLateral;

    // Start with "naive pursuit" (steer towards target)
    //    *steering = *localTarget;

    // If not heading generally towards the target, put on the brakes:
    // set the forward steering component to be strongly negative.
    return (ahead);
}


MlBoolean overshootingTarget (const SteerableMass *vehicle,
			      const MlVector3 *globalTarget,
			      const MlScalar futureTime)
{
    MlVector3 localTarget;

    // Transform global target point into local space (XXX note that
    // we've already done this in steerTowardGlobalTarget, and
    // modularity probably isn't a good enough reason to do it twice.)
    (*vehicle).localizeCoordinate(*globalTarget, localTarget);

    // We will overshoot the target if the distance we will travel by
    // futureTime is greater than the forward distance to the target.
    {
	MlScalar distanceTraveled = mlMul(futureTime, vehicleSpeed (vehicle));
	MlScalar forwardToTarget = (*vehicle).getForward(localTarget);
	return ((distanceTraveled > forwardToTarget) && 
		targetNearForwardAxis (vehicle, &localTarget));
    }

}
