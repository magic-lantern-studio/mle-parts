/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file cohere.cxx
 * @ingroup MleParts
 *
 * This file implements a class for a vehicle that exhibits
 * collective steering behavior.
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

//
// This package gives an actor the ability to cohere with other actors.


#include "mle/cohere.h"
#include "mle/behavlib.h"
#include "math/scalar.h"


// instance registry for package class
MlePtrArray Cohesion::registry(100);


//-------------------------------------------------------------------------


Cohesion::Cohesion  (Vehicle& v) : vehicle(v)
{
    // default parameters
    groupID = 0;
    nearDist = ML_SCALAR(3);
    frontAngle = ML_SCALAR(140.0);

    // register this instance
    registry.add(this);
}


//-------------------------------------------------------------------------
// destructor


Cohesion::~Cohesion (void)
{
    // unregister this instance
    registry.remove (this);
}


//-------------------------------------------------------------------------


void Cohesion::steeringForCohesion (MlVector3 *steering)
{
    // initialize returned steering value to zero
    (*steering) = MlVector3::zero;
    MlVector3  sumAll = MlVector3::zero;
    MlScalar count = ML_SCALAR_ZERO;
  

    // loop over all instance of this package class
    for ( int i = 0; i < registry.getSize(); i++ )
    {
        Cohesion *coh = (Cohesion *)registry[i];

        if ((groupID == coh->groupID) && (this != coh))
		{
	        // MlVector3 p1 = vehicle.position;
	        MlVector3 p2 = coh->vehicle.position;

	        // if object is close...
	        // if (pointsWithinDistance(nearDist, &p1, &p2))
	        if (vehicle.pointWithinSphereAngle (nearDist, frontAngle, &p2))
			{
	            sumAll += p2;
	            count += ML_SCALAR_ONE;
			}
		}
    }

    // steer towards center
    if (count != ML_SCALAR_ZERO)
	{
        MlVector3 center = sumAll / count;
        (*steering) = (center - vehicle.position);
	}
}
