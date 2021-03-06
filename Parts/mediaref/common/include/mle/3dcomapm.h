/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcomapm.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Colormap Media Reference
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

#ifndef __MLE_3DCOMAP_H_
#define __MLE_3DCOMAP_H_

// Include Runtime Engine header files.
#include "mle/MleMediaRef.h"
#include "mle/Mle3dColorMapMediaRef.h"


// XXX - This mediaref is an anachronism and should be done away with.
// It should be replaced with both a material property that allows color
// specification, and more appropriate use of mediarefs to load shade
// tables. bjw 7/11/96


/**
 * @brief 3D Colormap Table Media Reference.
 *
 * This class implements a Media Reference loader for colormap tables.
 */
class MLE_3DCOLORMAPMEDIAREF_API Mle3dColorMapMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dColorMapMediaRef);
    
  public:

    Mle3dColorMapMediaRef(void);

    virtual ~Mle3dColorMapMediaRef(void);

    void *read(void);
};


#endif /* __MLE_3DCOMAP_H_ */
