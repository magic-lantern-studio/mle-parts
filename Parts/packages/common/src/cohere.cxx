/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file cohere.cxx
 * @ingroup MleParts
 *
 * This file implements a class for a vehicle that exhibits
 * collective steering behavior.
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
