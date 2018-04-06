/** @defgroup MlMath Magic Lantern Math Library API */

/**
 * @file scalarbr.h
 * @ingroup MlMath
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file provides math utilities for the BRender platform.
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

#ifndef __MLE_SCALARBR_H_
#define __MLE_SCALARBR_H_

// Include Magic Lantern Math header files.
#include <math/scalar.h>

// Include BRender target header files.
#include <math/brapi.h>


//
// Conversion of MlScalar to and from BRender scalar.  Do this
// in a separate header file to avoid including BRender in <scalar.h>
//

// Convert from MlScalar to BRender scalar
inline br_scalar
mlScalarToBRScalar( MlScalar s )
{
#if (BASED_FLOAT || ! ML_FIXED_POINT)
	// At least one of these function calls is a no-op, and the other
	// is actually required.
	return BrFloatToScalar(mlScalarToFloat(s));
#else /* (BASED_FLOAT || ! ML_FIXED_POINT) */
//
// Both are fixed point, so an integer based conversion is OK.
// The BRender Fixed Point Scalar uses 16.16 format.
// Cannot shift by a negative amount, so cannot do what I really want,
// which is 
//	  return mlScalarGetValue(s) << (16 - ML_FIXED_RADIX);
//
#if (ML_FIXED_RADIX <= 16)
	return (br_scalar) mlScalarGetValue(s) << (16 - ML_FIXED_RADIX);
#else /* (ML_FIXED_RADIX <= 16) */
	return (br_scalar) mlScalarGetValue(s) >> (ML_FIXED_RADIX - 16);
#endif /* (ML_FIXED_RADIX <= 16) */

#endif /* (BASED_FLOAT || ! ML_FIXED_POINT) */
}

// Convert from BRender scalar to MlScalar.
inline MlScalar
mlBRScalarToScalar( br_scalar b )
{
#if (BASED_FLOAT || ! ML_FIXED_POINT)
	// At least one of these function calls is a no-op, and the other
	// is actually required.
	return mlFloatToScalar(BrScalarToFloat(b));
#else /* (BASED_FLOAT || ! ML_FIXED_POINT) */
//
// Both are fixed point, so an integer based conversion is OK.
// The BRender Fixed Point Scalar uses 16.16 format.
//
	MlScalar f;
#if (ML_FIXED_RADIX <= 16)
	f = mlScalarSetValue((long) b >> (16 - ML_FIXED_RADIX));
#else /* (ML_FIXED_RADIX <= 16) */
	f = mlScalarSetValue((long) b << (ML_FIXED_RADIX - 16));
#endif /* (ML_FIXED_RADIX <= 16) */
	return f;

#endif /* (BASED_FLOAT || ! ML_FIXED_POINT) */
}

#endif /* __MLE_SCALARBR_H_ */
