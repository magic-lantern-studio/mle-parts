/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Target.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines a class that displays the target for snapping operations.
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

#ifndef  __TARGET_H_
#define  __TARGET_H_

// Include Magic Lantern Rehearsal Player header files.
#include "SpotMarker.h"

// Declare external classes.
class SoSwitch;

/**
 * Target is a class that displays the target for snapping operations. It also
 * allows interactive placement of the target object using the middle
 * mouse button.
 */
class Target : public SpotMarker
{
  public:

    /**
	 * The default constructor.
	 */
	Target();

    /**
     * The destructor.
     */
    ~Target();
};

#endif /* __TARGET_H_ */
