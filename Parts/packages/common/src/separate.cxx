/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file separate.h
 * @ingroup MleParts
 *
 * This file defines a utility class for managing separation between
 * vehicles.
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


// This package gives an actor the ability to maintain a certain separation
// distance of other actors.

// Include Magic Lantern header files.
#include "mle/separate.h"
#include "mle/behavlib.h"
#include "math/scalar.h"

// allocate Separation registry
MlePtrArray Separation::registry(100);


Separation::Separation (Vehicle& v) : vehicle(v)
{
    // default parameters
    groupID = 0;
    nearDist = ML_SCALAR(3);
    frontAngle = ML_SCALAR(140.0);

    // register this instance
    registry.add(this);
}


Separation::~Separation (void)
{
    // unregister this instance
    registry.remove (this);
}


// Maintains a certain separation distance between vehicles to which this
// package is attached.  Returns a steering force vector intended to make
// the vehicle turn away from separation conflicts with other vehicles.
// If zero is returned, no separation maintenance is required.

// Separation is only maintained for other vehicles with the same groupID
// as this one.

// This is a heavy-weight proto implementation, it can be made faster....


void Separation::steeringForSeparation (MlVector3 *steering)
{
    // initialize returned steering value to zero
    (*steering) =  MlVector3::zero;

    // loop over all instance of this package class
    for ( int i = 0; i < registry.getSize(); i++ )
	{
	    Separation *sep = (Separation *)registry[i];

	    if ((groupID == sep->groupID) && (this != sep))
		{
		    MlVector3 p1 = vehicle.position;
		    MlVector3 p2 = sep->vehicle.position;

		    // if other vehicle is too close...
		    if (vehicle.pointWithinSphereAngle (nearDist,
							frontAngle,
							&p2))
			{
			    MlVector3 offset = p1 - p2;
			    MlScalar distance = offset.normalize();

			    // steer away from other vehicle
			    if (distance != ML_SCALAR_ZERO)
				(*steering) += (offset / distance);

			}
		}
	}
}
