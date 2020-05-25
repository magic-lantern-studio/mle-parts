/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmatlrm.h
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

#ifndef __MLE_3DMATLRM_H_
#define __MLE_3DMATLRM_H_


// Include Runtime Player header files.
#include "mle/MleMediaRef.h"
#include "mle/brendapi.h"

#include "mle/mlTypes.h"
#include "math/vector.h"


/**
 * @brief 3d Material Registry media reference.
 */
class Mle3dMaterialRegistryMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dMaterialRegistryMediaRef);
    
  public:

    Mle3dMaterialRegistryMediaRef();

    virtual               ~Mle3dMaterialRegistryMediaRef();

    br_material**         read();

    int                   getSize() { return m_materialRegistrySize; }

    br_material**         getList() { return m_materialRegistryList; }

    static br_material**  loadMaterialRegistry(char* filename, int& numMaterials);
    
  private:

    int                   m_materialRegistrySize;
    br_material**         m_materialRegistryList;
};

br_material*  mlBrGetMaterial(MlVector3 ambientColor, MlVector3 diffuseColor,
                              MlVector3 specularColor, MlScalar shininess, MlScalar transparency,
                              int lightingOn, int shadingSmooth,
                              int facesDoubleSided, int facesAlwaysInFront);

#endif /* __MLE_3DMATLRM_H_ */

