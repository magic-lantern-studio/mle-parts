/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file simpmass.cxx
 * @ingroup MleParts
 *
 * This file defines a class for a Simple Mass.
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
//#include <iostream.h>

#include "mle/simpmass.h"
#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"

#include "mle/behavlib.h"
#include "math/angle.h"
#include "math/sine.h"

// Arbitrarily defined coordinate system
const MlVector3 SimpleMass::forward(ML_SCALAR_ZERO,ML_SCALAR_ZERO,-ML_SCALAR_ONE);
const MlVector3 SimpleMass::up(ML_SCALAR_ZERO,ML_SCALAR_ONE,ML_SCALAR_ZERO);
const MlVector3 SimpleMass::right(ML_SCALAR_ONE,ML_SCALAR_ZERO,ML_SCALAR_ZERO);

SimpleMass::SimpleMass(MlVector3& p,MlRotation& o,MlBoolean schedFlag)
  : position(p),orientation(o)
{
	mass = ML_SCALAR_ONE;

	velocity.setValue(ML_SCALAR_ZERO,ML_SCALAR_ZERO,ML_SCALAR_ZERO);
	acceleration.setValue(ML_SCALAR_ZERO,ML_SCALAR_ZERO,ML_SCALAR_ZERO);

	lastAcceleration = MlVector3::zero;
	accelDamping = ML_SCALAR (0.7);

	viscosity = ML_SCALAR_ZERO; // default to zero
	friction = ML_SCALAR_ZERO;  // default to zero

	// Add the position update to the scheduler
	if ( schedFlag )
		g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,
			(MleSchedulerFunc)update,this,this);
}

SimpleMass::~SimpleMass()
{
	g_theTitle->m_theScheduler->remove(this);
}

void
SimpleMass::applyLocalForce(const MlVector3& force)
{
	// Put the force into global coordinate system
	MlVector3 global;
	globalizeVector(force,global);
 
	// Acceleration is F/m
	acceleration += global/mass;
}

void
SimpleMass::applyGlobalForce(const MlVector3& force)
{
	// Acceleration is F/m
	acceleration += force/mass;
}

void
SimpleMass::localizeVector(const MlVector3& src,MlVector3& dst) const
{
	orientation.inverse().multVec(src,dst);
}

void
SimpleMass::globalizeVector(const MlVector3& src,MlVector3& dst) const
{
	orientation.multVec(src,dst);
}

void
SimpleMass::localizeCoordinate(const MlVector3& src,MlVector3& dst) const
{
	/* Get the vector from the local origin to the coordinate */
	MlVector3 vec = src - position;

	/* And finish localization by rotating the vector */
	localizeVector(vec,dst);
}

void
SimpleMass::globalizeCoordinate(const MlVector3& src,MlVector3& dst) const
{
	/* Rotate into the global coordinate system */
	globalizeVector(src,dst);

	/* And finish by adding the position offset */
	dst += position;
}

// These functions, set{Forward,Right,Up}, give the passed vector a
//   component in the indicated direction of the requested weight.
//   For arbitrary coordinate basis vectors, this is effectively
//
//      v = v - v*v.dot(basis) + weight*basis;
//
//   but in practice, it really comes down to setting the right array
//   value.
void
SimpleMass::setForward(MlVector3& v,MlScalar weight) const
{
	v[2] = -weight;
}

void
SimpleMass::setRight(MlVector3& v,MlScalar weight) const
{
	v[0] = weight;
}

void
SimpleMass::setUp(MlVector3& v,MlScalar weight) const
{
	v[1] = weight;
}

// These functions get{Forward,Right,Up} return the magnitude of the
//   component in the indicated direction of the vector passed in.  For
//   arbitrary coordinate basis vectors, this is
//
//     return v.dot(basis);
//
//   but in practice, it is simply picking out the right array value.
MlScalar
SimpleMass::getForward(const MlVector3& v) const
{
	return -v[2];
}

MlScalar
SimpleMass::getRight(const MlVector3& v) const
{
	return v[0];
}

MlScalar
SimpleMass::getUp(const MlVector3& v) const
{
	return v[1];
}

MlBoolean 
SimpleMass::pointWithinSphereAngle (const MlScalar radius,
    const MlScalar angle, const MlVector3 *v) const
{
    MlVector3 G_forward;
    MlVector3 offset =  (*v) - position;
   
    MlBoolean WITHIN_RADIUS;

    WITHIN_RADIUS = VECTOR_WITHIN_MAGNITUDE (radius, offset);

    // if out of range, return false
    if (!WITHIN_RADIUS) 
        return ( FALSE );

    // if in range ...

    if (angle == ML_SCALAR(180.0))
        return ( TRUE ) ;
  
    globalizeVector(forward, G_forward);

    MlScalar Dot = mlDiv(offset.dot(G_forward), offset.length());
 
    MlScalar cosine = mlCos(mlAngleToRadians(angle));
  
    return (Dot >= cosine);

    // MlScalar Ang = mlMul(ML_SCALAR(360.0), mlAcos(Dot));
    //  return (Ang <= angle);
}     
  
void
SimpleMass::linearPredictPosition(const MlScalar futureFrames,
				  MlVector3 *futurePosition) const 
{
    // Predict our future position based on current velocity
    *futurePosition = position + (velocity * futureFrames);
}


// #define ANNOTATION
// #include "mle/3dannoa.h"
// #define ACCELERATION_ANNOTATION                                    \
// {                                                                  \
//     MlVector3 v;                                                   \
//     ANN_CLEAR (sm);                                                \
//     v = sm->position + (sm->acceleration * ML_SCALAR(150));        \
//     ANN_DRAW_LINE (sm, sm->position, v,                            \
// 		   ML_SCALAR_ONE, ML_SCALAR_ONE, ML_SCALAR_ZERO);  \
// }


void
SimpleMass::update(SimpleMass *sm)
{
    // If there is drag or friction, compute new acceleration
    if (sm->viscosity!=ML_SCALAR_ZERO 
        || sm->friction!=ML_SCALAR_ZERO) 
    {
        MlVector3 dragForce = MlVector3::zero;
        MlVector3 frictionForce = MlVector3::zero;
      
        if (sm->viscosity != ML_SCALAR_ZERO)
	        dragForce = sm->viscosity*sm->velocity;

        if (sm->friction != ML_SCALAR_ZERO)
		{
	        MlVector3 globalUp;
	        sm->globalizeVector(sm->up, globalUp);
	        // Strictly speaking, the mass in the following formulae 
            // should be mass*gravity
	        MlScalar pressure = mlMul(sm->getUp(globalUp), sm->mass);
	        MlVector3 globalForward;
	        sm->globalizeVector(sm->forward, globalForward);
	        frictionForce = mlMul(sm->friction, pressure) * globalForward;
		}
        // Get the accumulated external force from acceleration
        MlVector3 externalForce = sm->mass * sm->acceleration;

        // Compute total force
        MlVector3 totalForce = externalForce - dragForce - frictionForce;
        sm->acceleration = totalForce/sm->mass;
	}

    // Force accelDamping into range:
    if (sm->accelDamping < ML_SCALAR_ZERO) sm->accelDamping = ML_SCALAR_ZERO;
    if (sm->accelDamping > ML_SCALAR_ONE)  sm->accelDamping = ML_SCALAR_ONE;
    // Blend new acceleration into old acceleration
    if (sm->accelDamping != ML_SCALAR_ZERO)
	{
    	MlVector3::interpolate (sm->accelDamping,
				&(sm->acceleration),
				&(sm->lastAcceleration),
				&(sm->acceleration));

	    sm->lastAcceleration = sm->acceleration;
	}

    //	ACCELERATION_ANNOTATION;

    // Update velocity with acceleration
    sm->velocity += sm->acceleration;

    // Update position with velocity
    sm->position += sm->velocity;

    // No orientation adjustment is done
}

