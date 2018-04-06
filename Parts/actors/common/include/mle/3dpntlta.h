/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dlighta.cxx
 * @ingroup MleParts
 *
 * This file defines the actor for a Magic Lantern Point Light.
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

#ifndef __MLE_3DPNTLTA_H_
#define __MLE_3DPNTLTA_H_


#include "mle/3dlta.h" 

/**
 * @brief This class defines the Magic Lantern point lights.
 */
class Mle3dPointLightActor : public Mle3dBaseLightActor
{
    MLE_ACTOR_HEADER(Mle3dPointLightActor);
    
  public:

    // Constructor: initializes member properties nodeType to "LIGHT"
    // and lightType to "POINT".
    Mle3dPointLightActor();

    // Destructor: turns off the light.
    ~Mle3dPointLightActor();

    void init();

    // Push all properties (or state) to the Set.
    void update();

#ifdef MLE_REHEARSAL
  void resolveEdit(const char *);
#endif
    
};


#endif /* __MLE_3DPNTLTA_H_ */
