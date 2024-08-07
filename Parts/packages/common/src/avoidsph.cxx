/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file avoidsph.cxx
 * @ingroup MleParts
 *
 * This file implements a class for a vehicle that is capable of
 * steering around spherical objects.
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


// This package gives an actor the ability to avoid (to steer around)
// spherical obstacles.  It also provides detection and reaction for
// vehicle-obstacle collisions.
//
//-------------------------------------------------------------------------
//
// XXX issues:
//
// Maybe lastObstacleAvoided should be a static member of the class?
//
// Should COLLISION_FORCE_WEIGHTING and COLLISION_VELOCITY_DAMPING be
// member variables of the package?
//
//-------------------------------------------------------------------------

// Include system header files.
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/sphobs.h"
#include "mle/avoidsph.h"
#include "mle/behavlib.h"


//-------------------------------------------------------------------------


AvoidSphericalObstacle::AvoidSphericalObstacle (Vehicle& vehicleRef)
  : vehicle(vehicleRef)
{
}


// ------------------------------------------------------------------------
//
// Determines if any obstacles pose a risk of imminent collision to our
// vehicle.  If so, this function returns a steering direction vector that
// will turn the vehicle away from the potential collision.  Only obstacles
// that lie along the current flight path and with a certain distance are
// considered to pose a risk of collision.  If no potential collision is
// found this function returns a steering value of (0,0,0).


void AvoidSphericalObstacle::steeringForObstacleAvoidance (MlVector3 *steering)
{
    MlScalar threatDistance = vehicle.collisionThreatDistance();
    MlScalar vRadius = vehicle.radius;  // vehicle radius
    MlScalar minDistance = ML_SCALAR_MAX;  // "infinity"
    (*steering) = MlVector3::zero;
    lastObstacleAvoided = NULL;

    // loop over all obstacles
    for (int i = 0; i < SphericalObstacle::registry.getSize(); i++)
	{
	    SphericalObstacle *o = 
		(SphericalObstacle *)SphericalObstacle::registry[i];

	    MlScalar oRadius = mlMul(o->size, ML_SCALAR_HALF);  // obstacle
	    MlScalar cRadius = oRadius + vRadius;               // combined
	    MlScalar tRadius = cRadius + threatDistance;        // threat

	    // does the obstacle pose a collision threat to the vehicle? 
	    if (pointsWithinDistance (tRadius,
				      &(o->position),
				      &(vehicle.position)))
		{
		    // transform obstacle center to local space
		    MlVector3 lc;
		    vehicle.localizeCoordinate(o->position, lc);

		    // project center onto the lateral ("right/up") plane
		    MlVector3 localCenter2d = lc;
		    vehicle.setForward(localCenter2d, ML_SCALAR_ZERO);

		    // if the obstacle intersects the flight path...
		    if (VECTOR_WITHIN_MAGNITUDE (cRadius, localCenter2d))
			{
			    // local z of nearst part of obstacle
			    // (note that this should be implemented
			    // as a method on the obstacle class.)
			    MlScalar nearZ =
				(vehicle.getForward(lc) - 
				 mlSqrt ((mlSquare(cRadius) -
					  mlSquare(vehicle.getRight(lc)) - 
					  mlSquare(vehicle.getUp(lc)))));

			    // is obstacle in front of vehicle and nearer
			    // than any previously considered obstacles...
			    if ((nearZ > -vRadius) && (nearZ < minDistance))
				{
				// closest obstacle found yet
				    minDistance = nearZ;

				// steer away from obstacle center
				    (*steering) = -localCenter2d;

				// save in case anyone wants to know
				    lastObstacleAvoided = o;
				}
			}
		}
	}
}


// ------------------------------------------------------------------------
//
// Checks whether our Vehicle has penetrated any of the known
// SphericalObstacles.  If not, "displacement" and "opposingForce" are set
// to (0,0,0).  Otherwise "displacement" is set to a translation vector
// specifying how to move out of the interpentration, and "opposingForce"
// is set to the force needed to counteract all of the vehicle's velocity
// in the direction of the obstacle.  This corresponds to assuming the
// obstacle is much more massive than the vehicle.
//
// Note: both obstacle and vehicle are treated as their bounding spheres.


void AvoidSphericalObstacle::checkForObstaclePenetration (
    MlVector3 *displacement, MlVector3 *opposingForce)
{
    // initialize return values to zero
    (*displacement) = MlVector3::zero;
    (*opposingForce) = MlVector3::zero;

    // loop over all obstacles
    for ( int i = 0; i < SphericalObstacle::registry.getSize(); i++ )
	{
	    SphericalObstacle *o =
		(SphericalObstacle *)SphericalObstacle::registry[i];
	    MlScalar radius =
		mlMul(o->size, ML_SCALAR_HALF) + vehicle.radius;

	    // if within obstacle
	    if (pointsWithinDistance (radius,
				      &(o->position),
				      &(vehicle.position)))
		{
		    MlVector3 offset = (o->position) - (vehicle.position);
		    MlScalar separation = offset.length();

		    MlVector3 unitRadial(ML_SCALAR_ZERO,ML_SCALAR_ZERO,
					 ML_SCALAR_ONE);
		    if (separation != ML_SCALAR_ZERO)
			unitRadial = offset / separation;

		    // the distance between centers minus combined radii
		    // times the unit vector along the line between centers
		    (*displacement) = unitRadial * (separation - radius);

		    // a force normal to the surface of the obstacle, of a
		    // magnitude so as to exactly cancel out the vehicle's
		    // velocity component in the direction toward the
		    // center of the obstacle.
		    MlScalar m = vehicle.mass;
		    MlScalar rv = -unitRadial.dot(vehicle.velocity);
		    (*opposingForce) = unitRadial * mlMul(m, rv);
		}
	}
}


// ------------------------------------------------------------------------
//
// Determines if our vehicle is in collision with any obstacles in the
// world.  When interpenetration is detected, the vehicle's position and
// velocity is adjusted to model collision.


// COLLISION_WEIGHTING is some kind of vague approximation to the
// combined effects of non-elastic collision and moment of inertia.

#define COLLISION_FORCE_WEIGHTING ML_SCALAR(0.6)


// COLLISION_VELOCITY_DAMPING controls how much of the lateral velocity
// remains after a collision.  This models energy lost in the collision.

#define COLLISION_VELOCITY_DAMPING ML_SCALAR(0.9)


MlBoolean AvoidSphericalObstacle::checkForAndReactToCollisions (void)
{
    // check for collision (has vehicle penetrated any obstacle?)
    MlVector3 displacement, opposingForce;
    checkForObstaclePenetration (&displacement, &opposingForce);
    MlBoolean collision = ! displacement.isZero();

    // if so, adjust position and velocity of vehicle
    if (collision)
	{
	    // simple kinematic constraint: directly alter position
	    vehicle.position += displacement;

	    // apply constraint force impulse:
	    MlVector3 f = opposingForce * COLLISION_FORCE_WEIGHTING;
	    vehicle.applyGlobalForce(f);

	    // damp lateral velocity
	    // (should this be done via applyGlobalForce ?)
	    vehicle.velocity *= COLLISION_VELOCITY_DAMPING;
	}

    return collision;
}
