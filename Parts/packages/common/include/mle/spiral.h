/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file spiral.h
 * @ingroup MleParts
 *
 * This file defines a class for a Spiral behavior.
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
