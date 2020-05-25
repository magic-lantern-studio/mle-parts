/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dbboxc.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Bounding Box carrier.
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

#ifndef __MLE_3DBBOXC_H_
#define __MLE_3DBBOXC_H_


#include "math/vector.h"

class MleRole;

/**
 * @biref 3D Bounding Box property carrier.
 *
 * This class allows querying a role for a bounding box of its
 * representation. It will be implemented on each graphics substrate
 * in the appropriate way.
 */
class Mle3dBoundingBoxCarrier
{
  public:

    /**
	 * Puts the minimum of each coordinate component
     * in min, and the maximum of each coordinate component in max.
	 */
    static void getBounds(MleRole *role,MlVector3& min,MlVector3& max);
};

#endif /* __MLE_3DBBOXC_H_ */
