/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file spiral.h
 * @ingroup MleParts
 *
 * This file defines a class for a Spiral behavior.
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

#ifndef __MLE_SPIRAL_H_
#define __MLE_SPIRAL_H_

#include <math/scalar.h>
#include "mle/vehicle.h"

/**
 * @brief "Spiral down" behavior package
 */
class SpiralDown
{
  public:

    // constructor
    SpiralDown (Vehicle& vehicle);

    // destructor
    ~SpiralDown (void);
       
    // reference to the vehicle of the actor to which we are attached
    Vehicle& vehicle;
      
    // this is the function that does the work
    void steeringForSpiral (MlVector3 *steering);

    // factor to the radius of spiral
    MlScalar radius_factor;

    // factor to the speed of falling
    MlScalar fall_factor;

    // spiral direction
  private:

    MlBoolean spiralToRight;
    
};


#define mlRegisterSpiralDownPackageMembers(actor,package)          \
{ mlRegisterActorMember (actor, package.radius_factor, MlScalar);  \
  mlRegisterActorMember (actor, package.fall_factor, MlScalar);    }


#endif /* __MLE_SPIRAL_H_ */
