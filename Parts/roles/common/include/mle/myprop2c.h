/**
 * @file myprop2.h
 * @ingroup MleParts
 *
 * This file implements the carrier for two properties.
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

#ifndef __MLE_MYPROP2C_H_
#define __MLE_MYPROP2C_H_


#define Property1Type int
#define Property2Type int

class MleRole;

/**
 * @brief Example 2-member property carrier.
 *
 * Encapsulates the functionality for accessing scene graph variables.
 * Replace all occurrences of PropertyNName with the name of property N.
 * Replace all occurrences of PropertyNType with the type of property N.
 */
class MyTwoPropertyCarrier 
{
  public:

    static int set(MleRole* role, 
		   Property1Type Property1Name, Property2Type Property2Name);
    static int get(MleRole* role, 
		   Property1Type& Property1Name, Property2Type& Property2Name);
};

#endif /* __MLE_MYPROP2C_H_*/
