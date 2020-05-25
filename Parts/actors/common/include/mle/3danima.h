/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3danima.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Camera Actor.
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

#ifndef __ML_3DANIMA_H_
#define __ML_3DANIMA_H_

// Include Magic Lantern header files.
#include "mle/MleActor.h"
#include "mle/3dnodetp.h"
#include "mle/3drendp.h"
#include "mle/3dtranlp.h"
#include "mle/3dqrotp.h"
#include "mle/3dnscalp.h"
#include "mle/3danimrp.h"

#include "mle/3dscenep.h"


class Mle3dAnimationActor : public MleActor
{
    MLE_ACTOR_HEADER(Mle3dAnimationActor);
    
  public:

    Mle3dAnimationActor();

    virtual ~Mle3dAnimationActor();

    virtual void init();

    void         update();

    int          setupAnimation(int newAnimationNumber);

    MlTransform  computeAnimationAutoUpdateTransform(int parent);

    static void  behave(void* actor);
    
    
#ifdef MLE_REHEARSAL
    virtual void resolveEdit(const char *);
#endif

    // The member properties
    Mle3dNodeTypeProperty           nodeType;

    Mle3dRenderProperty             render;

    Mle3dTranslationProperty        position;

    Mle3dQuaternionRotationProperty orientation;

    Mle3dNonuniformScaleProperty    scale;

    Mle3dAnimationRegistryProperty  animationRegistry;

    Mle3dSceneGraphProperty         sceneGraph;
    
    // These will be a part of the frame animation package
    int                            animationPlayMode;

    int                            animationSpeed;

    int                            animationFirstFrame;

    int                            animationLastFrame;

    int                            animationNumber;

    int                            animationAutoUpdateFlag;

    int                            animationAutoUpdateParent;

    MlTransform                    animationAutoUpdateTransform;

    int                            frame;
   
  protected:

    int sceneGraphSize; 
    Mle3dAnimation*                animation;
    MlTransform**                  animationTransformList;
    char**                         animationObjectNameList;
};


#endif /* __MLE_3DANIMA_H_ */

