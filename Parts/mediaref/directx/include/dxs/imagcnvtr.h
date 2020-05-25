/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file imagcnvtr.h
 * @ingroup MleParts
 *
 * @author Mark S. Millard
 * @created Nov 20, 2007
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

#ifndef __MLE_IMAGCNVTR_H_
#define __MLE_IMAGCNVTR_H_


// Include Magic Lantern header files.
#include "mle/MleMediaRefConverter.h"


/**
 * @brief This class is an abstract class for managing Image Media
 * Reference Converters.
 */
class ImagMrefConverter : public MleMediaRefConverter
{
	/**
	 * Get the width of the retrieved file.
	 */
	virtual unsigned int getWidth(void) = 0;

	/**
	 * Get the height of the retrieved file.
	 */
	virtual unsigned int getHeight(void) = 0;
};


#endif /* __MLE_IMAGCNVTR_H_ */
