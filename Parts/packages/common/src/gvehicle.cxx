/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file gvehicle.h
 * @ingroup MleParts
 *
 * This file implements a class for a Generic Vehicle.
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


// Include system header files.
#include <stdio.h>


// Include Magic Lantern header files.
#include "mle/gvehicle.h"

#include "math/scalar.h"
#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"

#include "mle/behavlib.h"


GenericVehicle::GenericVehicle (MlVector3& p,
				MlRotation& o,
				MlScalar& s,
				MlBoolean schedFlag)
 : Vehicle(p, o, s, defaultbanker, defaultstabilizer, schedFlag), 
   defaultbanker(*this,FALSE),
   defaultstabilizer(*this,FALSE)
{
}

 
