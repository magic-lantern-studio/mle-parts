/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file align.h
 * @ingroup MleParts
 *
 * This file implements a class for a vehicle with collective
 * steering behavior.
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


// This package gives an actor the ability to align with other actors.

#include "mle/align.h"
#include "mle/behavlib.h"
#include "math/scalar.h"


// instance registry for package class
MlePtrArray Alignment::registry(100);


//-------------------------------------------------------------------------


Alignment::Alignment (Vehicle& v) : vehicle(v)
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


Alignment::~Alignment (void)
{
    // unregister this instance
    registry.remove (this);
}


//-------------------------------------------------------------------------

// Maintains alignment in orientation between vehicles to which this
// package is attached. Returns a steering force vector intended to
// make the instance vehicle head towards a general heading of other
// vehicles in the same group (specified by groupID).

void Alignment::steeringForAlignment (MlVector3 *steering)
{
    // initialize returned steering value to zero
    (*steering) = MlVector3::zero;

    MlVector3 generalHeading = MlVector3::zero;
    MlVector3 Heading;
    MlScalar count = ML_SCALAR_ZERO;
    			 
    // loop over all instance of this package class
    for ( int i = 0; i < registry.getSize(); i++ )
	{
	  Alignment *align = (Alignment *)registry[i];
	  
	  if ((groupID == align->groupID) && (this != align))
	    {
	      //  MlVector3 v1 = vehicle.position;
	      MlVector3 v2 = align->vehicle.position;
	      
	      // if close to each other ...
	      if (vehicle.pointWithinSphereAngle (nearDist, 
						  frontAngle, &v2))
		{
		  MlVector3 localOrientation = align->vehicle.forward;
		  align->vehicle.globalizeVector(localOrientation, 
						 Heading);
		  generalHeading += Heading;
		  count += ML_SCALAR_ONE;
		}
	    }
	}
    
    if (count != ML_SCALAR_ZERO)
      // return global steering vector
      (*steering) = (generalHeading / count);
    	
}
