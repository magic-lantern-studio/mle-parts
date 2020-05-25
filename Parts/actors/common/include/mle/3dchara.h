/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dchara.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Character Animation Actor.
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

#ifndef __MLE_3DCHARA_H_
#define __MLE_3DCHARA_H_

#include "mle/3danima.h"
#include "mle/3dcharrp.h"


class Mle3dCharacterActor : public Mle3dAnimationActor
{
    MLE_ACTOR_HEADER(Mle3dCharacterActor);
    
  public:

    Mle3dCharacterActor();

    virtual ~Mle3dCharacterActor();

    virtual void init();

    void         update();

    void         checkForEvents();

    static void  behave(void* actor);
    
    
#ifdef MLE_REHEARSAL
    virtual void resolveEdit(const char *);
#endif

    Mle3dCharacterRegistryProperty  characterRegistry;

    int          characterNumber;

    int          snippetNumber;

  protected:

    Mle3dCharacter* character;
    Mle3dSnippet*   snippet;
};


#endif /* __MLE_3DCHARA_H_ */

