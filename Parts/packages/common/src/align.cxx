/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file align.h
 * @ingroup MleParts
 *
 * This file implements a class for a vehicle with collective
 * steering behavior.
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
