/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brmlmref.h
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

#ifndef __MLE_BRMLMREF_H_
#define __MLE_BRMLMREF_H_

// Include Runtime Player header files.
#include "mle/MleMediaRef.h"
#include "mle/brendapi.h"

/**
 * @brief A Media Reference loader for BRender models.
 */
class brmlmref : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(brmlmref);
    
  public:

    brmlmref(void);

    virtual ~brmlmref(void);

    br_model *read(void);
};

#endif /* __MLE_BRMLMREF_H_ */
