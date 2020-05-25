/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file statcam.h
 * @ingroup MleParts
 *
 * This file defines the class for a Static Camera Actor.
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

#ifndef __MLE_STATCAM_H_
#define __MLE_STATCAM_H_

// Include Magic Lantern header files.
#include "mle/MleActor.h"
#include "math/scalar.h"
#include "math/vector.h"
#include "math/rotation.h"

class Mle3dSet;

class StaticCamera : public MleActor
{
    MLE_ACTOR_HEADER(StaticCamera);

  public:

    StaticCamera(void);
    
    // Declare property member variables.
    MlVector3 position;

    MlRotation orientation;

    MlScalar nearClipping;

    MlScalar farClipping;
    
    virtual void init(void);
    
    static void update(StaticCamera *mycamera);
    
#ifdef MLE_REHEARSAL
    virtual void resolveEdit(const char *property);
#endif

  protected:

    // This is not quite right.  The reason is that this actor is
    //   supposed to control a camera at runtime, but it holds
    //   a pointer to a forum object that defines camera control
    //   only at rehearsal time.  So this should be some more specific
    //   subclass.  The problem is how to define this in a way that
    //   works for both mastering and rehearsal.
	Mle3dSet *m_set;

};

#endif /* __MLE_STATCAM_H_ */
