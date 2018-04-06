/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmodela.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Model Actor.
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

#ifndef __MLE_3DMODELA_H_
#define __MLE_3DMODELA_H_

//Title:    3D MODEL ACTOR

#include "mle/MleActor.h"
#include "mle/3dtranlp.h"
#include "mle/3dqrotp.h"
#include "mle/3duscalp.h"
#include "mle/3dmodelp.h"
#include "mle/3dcomapp.h"
#include "mle/3dtexmpp.h"

#include "mle/mlTypes.h"

class Mle3dModelActor : public MleActor
{
    MLE_ACTOR_HEADER(Mle3dModelActor);
    
  public:

    Mle3dModelActor();

    void init();

    void update();

    void resolveEdit(const char *);
    

    // The member properties
    Mle3dTranslationProperty        position;

    Mle3dQuaternionRotationProperty orientation;

    Mle3dUniformScaleProperty       scale;

    Mle3dModelProperty              model;

    Mle3dTextureMapProperty         textureMap;

    Mle3dColorMapProperty           colorMap;

};


#endif /* _MLE_3DMODELA_H_ */

