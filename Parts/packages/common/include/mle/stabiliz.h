/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file stabiliz.h
 * @ingroup MleParts
 *
 * This file defines a class for a Stabilizer.
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

#ifndef __MLE_STABILIZER_H_
#define __MLE_STABILIZER_H_

#include "mle/simpmass.h"
#include "mle/MleScheduler.h"

/**
 * @brief Stabilizer
 *
 * This package limits the angular velocity of a Vehicle.
 *
 * Vehicles are based on SimpleMass which is a point mass model.  A
 * point mass has no moment of inertia and so is capable of very high
 * turning rates.  The Stabilizer package limits the rate of turn.
 * 
 * @see Vehicle
 */
class Stabilizer
{
  public:

    // Constructor: initializes reference to SimpleMass and optionally
    // schedules the vehicle's update function.
    Stabilizer(SimpleMass& sm,MlBoolean schedFlag = TRUE);

    // Destructor: removes scheduler items associated with this instance.
    ~Stabilizer();
    
    // A reference to the mass associated with this Stabilizer.
    SimpleMass& sm;
    
    // Per frame update function (optionally scheduled by contructor).
    static void update(Stabilizer *s);
};

#endif /* __MLE_STABILIZER_H_ */
