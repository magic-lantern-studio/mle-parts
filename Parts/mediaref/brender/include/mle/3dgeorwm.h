/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dgeorwm.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Geometry Media Reference
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

#ifndef __MLE_3DGEORWM_H_
#define __MLe_3DGEORWM_H_

// Include Runtime Player header files.
#include <mle/MleMediaRef.h>

// Include parent class header file.
#include <mle/3dgeomrm.h>

/**
 * @brief 3d Geometry Registry web-capable media reference.
 */
class Mle3dGeometryRegistryWebMediaRef : public Mle3dGeometryRegistryMediaRef
{
	MLE_MEDIAREF_HEADER(Mle3dGeometryRegistryWebMediaRef);
  
  // These are all we need from the base class.
  public:

    Mle3dGeometryRegistryWebMediaRef(void);
  
    void setCallback( void*(*)(void*, void*), void*, void*);

    static void* apply(Mle3dGeometryRegistryWebMediaRef *, void *);

    void* read(void);

    void* (*callback)(void*, void*);

  protected:

    void *client;
    void *clientData;

};
#endif /* __MLE_3DGEORWM_H_ */
