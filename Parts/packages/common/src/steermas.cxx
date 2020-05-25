/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file steermass.h
 * @ingroup MleParts
 *
 * This file defines a class for a Steerable Mass.
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
#include "mle/steermas.h"
#include "mle/MleScheduler.h"

SteerableMass::SteerableMass(MlVector3& position,MlRotation& orientation,
	MlBoolean schedFlag)
  : SimpleMass(position,orientation,schedFlag)
{
}

void
SteerableMass::thrust(MlScalar value)
{
	applyLocalForce(value*forward);
}

void
SteerableMass::turn(MlScalar value)
{
	applyLocalForce(value*right);
}

void
SteerableMass::climb(MlScalar value)
{
	applyLocalForce(value*up);
}
