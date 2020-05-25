/** @defgroup MlMath Magic Lantern Math Library API */

/**
 * @file brapi.h
 * @ingroup MlMath
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file provides configuration for the BRender platform.
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

#ifndef __MLE_BRAPI_H_
#define __MLE_BRAPI_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Math header files.
#include <math/scalar.h>

// Set math macros for fixed-point or floating-point.
#if ML_FIXED_POINT
#define BASED_FIXED 1
#define BASED_FLOAT 0
#else /* ML_FIXED_POINT */
#define BASED_FIXED 0
#define BASED_FLOAT 1
#endif /* ML_FIXED_POINT */

// Include BRender header files
#include "brender.h"

#endif /* __MLE_BRAPI_H_ */
