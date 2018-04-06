//Filename: 3dAnimD.h
//Title:    3D ANIMATION DELEGATE
/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3danimd.h
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Animation Role.
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


#ifndef __3DANIMD_H_
#define __3DANIMD_H_

#include "mle/3dgeomd.h"
#include "mle/3danim_m.h"


class Mle3dAnimationRole : public Mle3dGeometryRole
{
    MLE_ROLE_HEADER(Mle3dAnimationRole);

  public:

    Mle3dAnimationRole(MleActor * actor);
    ~Mle3dAnimationRole();
    void init(void);
  
    // Update function to run every cycle.
    static void             update(void *client);

    Mle3dAnimationRegistry* getAnimationRegistry();
    Mle3dAnimationRegistry* setAnimationRegistry(char* filename);

    Mle3dAnimation*         setCurrentAnimation(int animationNumber);
    Mle3dAnimation*         getCurrentAnimation();


  protected:

    br_matrix34**           setupSceneGraph();

  private:

    Mle3dAnimation*         animation;
    Mle3dAnimationRegistry* animationRegistry;
    Mle3dSequence**         sequenceList;
    int                     updateActorModel(long i,Mle3dSequence* sequence);
    Mle3dAnimation*         setCurrentAnimation(Mle3dAnimation* newAnimation);
    void                    updateNumAnimations();
    int                     currentFrame;
    int                     currentAnimation;
    int                     previousFrame;
    int                     initialized;
};



#endif /* __3DANIMD_H_ */ 
