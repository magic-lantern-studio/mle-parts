/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmatlm.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Material Media Reference
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

#ifndef __MLE_3DMATLM_H_
#define __MLE_3DMATLM_H_


// Include Runtime Player header files.
#include "mle/MleMediaRef.h"
#ifdef WIN32
#include "brender.h"
#else
#include "mle/brendapi.h"
#endif /* WIN32 */


#include "mle/mlTypes.h"
#include "math/vector.h"


class Mle3dMaterialMedia : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dMaterialMedia);

  public:

    Mle3dMaterialMedia();
    virtual               ~Mle3dMaterialMedia(void);
    br_material**         read(void);
    int                   getMaterialCount() { return materialCount; }
    br_material**         getMaterialList() { return materialList; }
    static br_material**  loadMaterialRegistry(char* filename, int& numMaterials);

  private:

    int           materialCount;
    br_material** materialList;
};

br_material*  mlBrGetMaterial(MlVector3 ambientColor, MlVector3 diffuseColor,
                              MlVector3 specularColor, MlScalar shininess, MlScalar transparency,
                              int lightingOn, int shadingSmooth,
                              int facesDoubleSided, int facesAlwaysInFront);
br_uint_8     mlBrGetRenderStyle(MleRenderStyle renderStyle);



#endif /*__MLE_3DMATLM_H_ */

