/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3ddirlta.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Directional Light.
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

#ifndef __MLE_3DDIRLTA_H_
#define __MLE_3DDIRLTA_H_

#include "mle/3dlta.h"
 
/**
 * @brief This class defines the Magic Lantern directional lights.
 */
class Mle3dDirectLightActor : public Mle3dBaseLightActor
{
    MLE_ACTOR_HEADER(Mle3dDirectLightActor);
    
  public:

    // Constructor: initializes member properties nodeType to "LIGHT"
    // and lightType to "DIRECTIONAL".
    Mle3dDirectLightActor();

    // Destructor: turns off the light.
    ~Mle3dDirectLightActor();

    void init();

    //  push all properties (or state) to the forum
    void update();

#ifdef MLE_REHEARSAL
    void resolveEdit(const char *);
#endif
};


#endif /* __MLE_3DDIRLTA_H_ */
