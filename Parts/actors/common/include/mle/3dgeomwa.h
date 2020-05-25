/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dgeomwa.h
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Geometry Actor
 * with Web geometry registry.
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

#ifndef __MLE_3DGEOMWA_H_
#define __MLE_3DGEOMWA_H_

#include "mle/MleActor.h"
#include "mle/3drstylp.h"
#include "mle/3dtranlp.h"
#include "mle/3derotp.h"
#include "mle/3dnscalp.h"
#include "mle/3dgeorwp.h"	/* The Web version of this property */


/**
 * @brief 3d geometry actor with web geometry registry.
 */
class Mle3dGeometryWebActor : public MleActor
{
    MLE_ACTOR_HEADER(Mle3dGeometryWebActor);

  public:

    void init();

    void update();
    
#ifdef MLE_REHEARSAL
    void resolveEdit(const char *);
#endif

    // The member properties

    Mle3dRenderStyleProperty         renderStyle;

    Mle3dTranslationProperty         translation;

    Mle3dEulerAngleRotationProperty  rotation;

    Mle3dNonuniformScaleProperty     scale;

    Mle3dGeometryRegistryWebProperty geometryRegistry;

};


#endif /* __MLE_3DGEOMWA_H_ */

