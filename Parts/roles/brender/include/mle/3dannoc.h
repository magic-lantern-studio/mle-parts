/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dannoc.h
 * @ingroup MleParts
 *
 * This file contains the class for an interface to draw line segment
 * annotation.
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

#ifndef __MLE_3DANNOC_H_
#define __MLE_3DANNOC_H_

#include "math/vector.h"

/**
 * @brief This class encapsulates the interface to draw line segment annotation.
 *
 * This class encapsulates the interface to draw line segment annotation.
 * It will be implemented on each graphics substrate in the appropriate
 * way.
 * 
 * The interface can handle multiple contexts, where each context is a
 * group of related line segments.  It is customary for the calling object
 * to use its pointer as the context.
 * 
 * @see Mle3dRole
 */
class Mle3dAnnotationCarrier
{
  public:

    // clear() removes all geometry associated with a context.
    static void clear(MleRole *role,void *context);
    
    // drawLine() adds a line segment to the display list for a context.
    static void drawLine(MleRole *role,void *context,
			 const MlVector3& from,const MlVector3& to,
			 float red,float grn,float blu);
};

#endif /* __MLE_3DANNOC_H_ */
