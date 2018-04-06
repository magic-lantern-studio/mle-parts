/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcharrm.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Character Registry Media Reference
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

#ifndef __MLE_3DCHARRM_H_
#define __MLE_3DCHARRM_H_

#ifdef psx
#define FILE int
#endif

#include <stdio.h>

#include "mle/mlTypes.h"
#include "mle/3dchar.h"
#include "math/transfrm.h"

// Include Runtime Player header files.
#include "mle/MleMediaRef.h"


/**
 * @brief 3d Character Registry Media Reference
 */
class Mle3dCharacterRegistryMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dCharacterRegistryMediaRef);
    
  public:

    Mle3dCharacterRegistryMediaRef();

    virtual                  ~Mle3dCharacterRegistryMediaRef();

    Mle3dCharacterRegistry*   read();

    Mle3dCharacterRegistry*   getCharacterRegistry() { return m_characterRegistry; }
    
  private:

    Mle3dCharacterRegistry*  m_characterRegistry;
};

#endif /* __MLE_3DCHARRM_H_*/
