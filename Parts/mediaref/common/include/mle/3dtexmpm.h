/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtexmpm.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Texture Map Media Reference
 * targeting the BRender platform.
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

#ifndef __MLE_3DTEXMPM_H_
#define __MLE_3DTEXMPM_H_

// Include Runtime Engine header files.
#include "mle/MleMediaRef.h"
#include "mle/Mle3dTextureMapMediaRef.h"


/**
 * @brief A Media Reference loader for texture maps.
 */
class MLE_3DTEXTUREMAPMEDIAREF_API Mle3dTextureMapMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dTextureMapMediaRef);
    
  public:

    Mle3dTextureMapMediaRef(void);

    virtual ~Mle3dTextureMapMediaRef(void);

    void *read(void);
};

#endif /* __MLE_3DTEXMPM_H_ */
