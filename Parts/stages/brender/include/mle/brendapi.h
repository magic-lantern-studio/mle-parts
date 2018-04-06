/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brendapi.h
 * @ingroup MleParts
 *
 * This file is a top-level include file for the BRender Stage.
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

#ifndef __MLE_BRENDAPI_H_
#define __MLE_BRENDAPI_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Math header files.
#include <math/scalar.h>

// Set math macros for fixed-point or floating-point.

#if 0
#if ML_FIXED_POINT
#define BASED_FIXED 1
#define BASED_FLOAT 0
#else /* ! ML_FIXED_POINT */
#define BASED_FIXED 0
#define BASED_FLOAT 1
#endif /* ML_FIXED_POINT */
#endif /* 0 */


#if ML_FIXED_POINT
#ifndef BASED_FIXED
#define BASED_FIXED 1
#endif
#ifndef BASED_FLOAT
#define BASED_FLOAT 0
#endif
#else /* ! ML_FIXED_POINT */
#ifndef BASED_FIXED
#define BASED_FIXED 0
#endif
#ifndef BASED_FLOAT
#define BASED_FLOAT 1
#endif
#endif /* ML_FIXED_POINT */


// Include BRender header files
#include "brender.h"

#endif /* __MLE_BRENDAPI_H_ */
