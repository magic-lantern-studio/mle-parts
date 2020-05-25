/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file myprop1.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a single property.
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

#ifndef __MLE_MYPROP1C_H_
#define __MLE_MYPROP1C_H_

#define PropertyType int

class MleRole;

/**
 * @brief Example 1-member property carrier.
 *
 * Encapsulates the functionality for accessing scene graph variables.
 * Replace all occurrences of PropertyNName with the name of property N.
 * Replace all occurrences of PropertyNType with the type of property N.
 */
class MyOnePropertyCarrier 
{
  public:

    static int set(MleRole* role, PropertyType PropertyName);
    static int get(MleRole* role, PropertyType& PropertyName);
};

#endif /* __MLE_MYPROP1C_H_ */
