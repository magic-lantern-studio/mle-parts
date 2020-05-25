/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pe.cxx
 * @ingroup MleParts
 *
 * This file implements a class for a pursuer or evader vehicle.
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

// ------------------------------------------------------------------------
//
// This package supplies basic pursuit and evasion ("pe") functionality to
// an actor.  It supports static and dynamics versions of both pursuit and
// evasion.  Static versions assume the target is a fixed location in
// space, dynamic versions assume the target is another moving vehicle
//
// Nothing is scheduled by this package, calls must be made from the
// actor's own scheduled code into functions provided by this package.
//
// ------------------------------------------------------------------------

// Include Magic Lantern header files.
#include "mle/pe.h"
#include "mle/behavlib.h"


// ------------------------------------------------------------------------
// constructor


PursueEvade::PursueEvade (Vehicle& vehicleRef) : vehicle(vehicleRef)
{
    predictionGainForPursuit = DEFAULT_PREDICTION_GAIN;
    predictionGainForEvasion = DEFAULT_PREDICTION_GAIN;
}


// ------------------------------------------------------------------------
// Noise is being added to steering direction to prevent zero steering in
// certain rare cases caused by perfect alignment, such as a pursuit target
// directly behind a vehicle.  The magnitude of the noise is between 1% and
// 2% relative to the steering "force" vector.


#define ADD_STEERING_NOISE(steering)                \
{                                                   \
    MlVector3 noise;                                \
    MlScalar mag = (steering).approximateLength();  \
    setToRandomPointInCube (&noise, mag/128);       \
    (steering) += noise;                            \
}


// ------------------------------------------------------------------------
// This is Roy Hashimoto's "desired velocity" approach.  It assumes that in
// the absence of other refinements, we would like to be heading from our
// current position toward the target location at our maximum speed.  So
// the desired acceleration is the difference between our current and
// desired velocity.  Normally this vector will be significantly truncated
// to correspond to the force available to the vehicle during any single
// simulation step.
//
// Note: this functionality is about equivalent to steerTowardGlobalTarget
// from behavelib.cxx.


void PursueEvade::steeringForStaticPursuit  (const MlVector3 *location,
	MlVector3 *steering)
{
    MlVector3 targetOffset = (*location) - vehicle.position;
    MlVector3 desiredVelocity = targetOffset;
    truncateVectorLength (&desiredVelocity, vehicle.maxSpeed);
    MlVector3 desiredAcceleration = desiredVelocity - vehicle.velocity;
    (*steering) = desiredAcceleration * vehicle.mass;
    ADD_STEERING_NOISE (*steering);
    target = *location;
}


// ------------------------------------------------------------------------
// Steer away from a static global location.


void PursueEvade::steeringForStaticEvasion  (const MlVector3 *location,
					     MlVector3 *steering)
{
    // simply negate the steering for static pursuit
    steeringForStaticPursuit (location, steering);
    (*steering).negate();
}


// ------------------------------------------------------------------------
// This answers the question: is pursuer "bow riding" on its quarry?  The
// term "bow riding" refers to a behavior of dolphins, they surf on the bow
// wave formed by large ships.  Here the term means when the pursuer is in
// front of and heading approximately in the same direction as its quarry.
// Bow riding is a "bug" of naive predictive pursuit, this routine is used
// to detect and prevent it.

// Note: This is quick initial implementation.  There are probably faster
// ways to do it.  Maybe this should be a method on class Vehicle?


MlBoolean isBowRiding (const Vehicle *pursuer, const Vehicle *quarry)
{
    // the pursuer's position in terms of the quarry's local space:
    MlVector3 ppiqs;
    quarry->localizeCoordinate(pursuer->position, ppiqs);

    MlScalar f = quarry->getForward (ppiqs);
    MlScalar r = quarry->getRight (ppiqs);
    MlScalar u = quarry->getUp (ppiqs);

    static MlScalar slope = ML_SCALAR (0.5);

    if ((f > ML_SCALAR_ZERO) &&
	    (mlDiv (mlAbs(r), f) < slope) &&
	    (mlDiv (mlAbs(u), f) < slope))
	{
	    MlVector3 pForward, qForward;
	    pursuer->globalizeVector (pursuer->forward, pForward);
	    quarry->globalizeVector  (quarry->forward,  qForward);
	    static MlScalar correlation = ML_SCALAR (0.8);

	    return (pForward.dot(qForward) > correlation);
	}
    else
	    return FALSE;
}


// ------------------------------------------------------------------------
// Internal utilities used by dynamic pursuit and evasion.  One does simple
// prediction of the future position of the "opponent" vehicle based on its
// velocity and the distance between the two vehicles.  Currently ignores
// relative heading of the two vehicles, except to prevent "bow riding" in
// predictive pursuit.  Then steeringDynamicPursuitInternal finds steering
// to that predicted position as a static target.


void PursueEvade::predictionForDynamicPursuit (const Vehicle *opponent,
					       const MlScalar predictGain,
					       const MlBoolean noBowRide,
					       MlVector3 *futurePosition)
{
    // in the bow-riding case ...
    // (that is, when we both: want to avoid it and it is detected)
    if (noBowRide && isBowRiding (&vehicle, opponent))
	{
	    // ...do no prediction.
	    *futurePosition = opponent->position;
	}
    else
	{
	    // ...otherwise estimate lead time based on distance
	    MlVector3 offset = opponent->position - vehicle.position;
	    MlScalar distance = offset.approximateLength();
	    MlScalar time = mlMul (distance, predictGain);

	    // predict opponent's position "time" frames in the future
	    (*opponent).linearPredictPosition (time, futurePosition);
	}
}


void PursueEvade::steeringDynamicPursuitInternal (const Vehicle *opponent,
						  const MlScalar predictGain,
						  const MlBoolean noBowRide,
						  MlVector3 *steering)
{
    // predict opponent's future position
    MlVector3 prediction;
    predictionForDynamicPursuit (opponent, predictGain,
				 noBowRide, &prediction);

    // steer towards that location
    steeringForStaticPursuit (&prediction, steering);
}


// ------------------------------------------------------------------------
// Steer so as to chase or catch a moving vehicle.


void PursueEvade::steeringForDynamicPursuit (const Vehicle *opponent,
					     MlVector3 *steering)
{
    steeringDynamicPursuitInternal (opponent,
				    predictionGainForPursuit,
				    TRUE,
				    steering);
}


// ------------------------------------------------------------------------
// Steer to escape from a moving vehicle.


void PursueEvade::steeringForDynamicEvasion (const Vehicle *opponent,
					     MlVector3 *steering)
{
    // simply negate the steering for static pursuit
    steeringDynamicPursuitInternal (opponent, 
				    predictionGainForEvasion,
				    FALSE, 
				    steering);
    (*steering).negate();
}


// ------------------------------------------------------------------------
// Steer to chase a moving vehicle by aiming at a point offsetDist away
// from it.  That is: fly toward, but not directly at, the other vehicle.


void PursueEvade::steeringForOffsetPursuit (const Vehicle *opponent,
	const MlScalar offsetDist,
	MlVector3 *steering)
{
    // if we are close to the opponent do offset pursuit, otherwise 
    // use efficient dynamic (agressively predictive) pursuit.
    MlVector3 flatCenter;
    MlVector3 localCenter;
    MlVector3 futurePosition;

    // are we close to the opponent?
    if (pointsWithinDistance (offsetDist * 2,
			      &(vehicle.position),
			      &(opponent->position)))
	{
	    // transform predicted opponent center to local space
	    predictionForDynamicPursuit (opponent,
					 predictionGainForPursuit,
					 TRUE, &futurePosition);
	    vehicle.localizeCoordinate(futurePosition, localCenter);

	    // is opponent in front of us?
	    if (vehicle.getForward (localCenter) > ML_SCALAR_ZERO)
		{
		    // project to local side-up plane (forward = 0 plane)
		    flatCenter = localCenter;
		    vehicle.setForward (flatCenter, ML_SCALAR_ZERO);

		    MlScalar flatDist = flatCenter.length();
		    MlVector3 target = flatCenter;

		    // find offset nearest local origin, at distance
		    // "offsetDist" from flatCenter.
		    target *= mlDiv ((flatDist - offsetDist), flatDist);

		    // add target offset to local center of opponent
		    target += localCenter;
    
		    // globalize local target
		    vehicle.globalizeCoordinate(target, target);

		    // compute steering for target.
		    steeringForStaticPursuit (&target, steering);
		}
	    else
		{
		    // just orbit opponent (radial steering force)
		    (*steering) = opponent->position - vehicle.position;
		}
	}
    else
	{
	    // we are far away from opponent, use standard dynamic pursuit
	    steeringForDynamicPursuit (opponent, steering);
	}
}


// ------------------------------------------------------------------------
// Steer towards a global location, eventually slowing so as to come to a
// stop when we reach the location.  The vehicle will begin to slow when
// its travel time to the target is within "decelerationTime".  This value
// is specified in units of frames (simulation steps).  The conversion from
// time to distance assumes the vehicle is moving at its maximum speed.


void PursueEvade::steeringForGoToStopAt (const MlVector3 *location,
	const MlScalar decelerationTime,
	MlVector3 *steering)
{
    // find the desired velocity toward the target
    MlVector3 desired_velocity = *location - vehicle.position;

    // slow down linearly during decelerationTime frames
    MlScalar distance = desired_velocity.approximateNormalize();
    MlScalar speed = mlDiv (distance, decelerationTime);

    // don't go over the speed limit
    speed = MLE_MIN (speed, vehicle.maxSpeed);
    desired_velocity *= speed;

    // steering force is difference between desired and actual velocity
    // (should these be adjusted by mass?  would it make any difference?)
    (*steering) = desired_velocity - vehicle.velocity;
}
