/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmatrwm.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Material Registry Media Reference
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

#ifndef __MLE_3DMATRWM_H_
#define __MLE_3DMATRWM_H_


// Include Runtime Player header files
#include <fw/mediaref.h>

// include parent class header file
#include <fw/3dmatlrm.h>

/**
 * @brief 3d Material Registry Web-capable Media Reference.
 */
class Mle3dMaterialRegistryWebMediaRef : public Mle3dMaterialRegistryMediaRef
{
	MLE_MEDIAREF_HEADER(Mle3dMaterialRegistryWebMediaRef);
  
  // These are all we need from the base class.
  public:

    Mle3dMaterialRegistryWebMediaRef();
};


#endif /* __MLE_3DMATRWM_H_ */
