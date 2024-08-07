/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file separate.h
 * @ingroup MleParts
 *
 * This file defines a utility class for managing separation between
 * vehicles.
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
