/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dposa.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Position Orientation and Scale Actor.
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

#ifndef __MLE_3DPOSA_H_
#define __MLE_3DPOSA_H_

#include "mle/MleActor.h"
#include "mle/3dnodetp.h"
#include "mle/3dtranlp.h"
#include "mle/3dqrotp.h"
#include "mle/3duscalp.h"


class Mle3dPOSActor : public MleActor
{
    MLE_ACTOR_HEADER(Mle3dPOSActor);
    
  public:

    Mle3dPOSActor();

    ~Mle3dPOSActor();

    void init();

    void update();

    static void behave(void* client);

#ifdef MLE_REHEARSAL
    void resolveEdit(const char *);
#endif
    
    // The member properties

    Mle3dNodeTypeProperty           nodeType;

    Mle3dTranslationProperty        position;

    Mle3dQuaternionRotationProperty orientation;

    Mle3dUniformScaleProperty       scale;

};


#endif /* __MLE_3DPOSA_H_ */

