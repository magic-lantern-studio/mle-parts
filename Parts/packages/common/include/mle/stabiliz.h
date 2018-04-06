/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file stabiliz.h
 * @ingroup MleParts
 *
 * This file defines a class for a Stabilizer.
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
