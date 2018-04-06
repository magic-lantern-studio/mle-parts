/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmodelm.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Model Media Reference
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

#ifndef __MLE_3DMODELM_H_
#define __MLE_3DMODELM_H_


// Include Magic Lantern Runtime Engine header files.
#include "mle/MleMediaRef.h"
#include "mle/Mle3dModelMediaRef.h"


/**
 * @brief 3D Model Media Reference.
 */
class MLE_3DMODELMEDIAREF_API Mle3dModelMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dModelMediaRef);
    
  public:

    Mle3dModelMediaRef();

    virtual ~Mle3dModelMediaRef();

    void *read();
};

#endif /* __MLE_3DMODELM_H_ */
