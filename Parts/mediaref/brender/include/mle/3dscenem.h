/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dscenem.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Scene Graph Media Reference
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

#ifndef __MLE_3DSCENEM_H_
#define __MLE_3DSCENEM_H_

// include Runtime Player header files
#include "mle/MleMediaRef.h"
#include "mle/mlTypes.h"

#define MLE_MAX_NODES   256

/**
 * @brief 3d Scene Graph media reference 
 */
class Mle3dSceneGraphMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dSceneGraphMediaRef);

  public:

    Mle3dSceneGraphMediaRef();

    virtual   ~Mle3dSceneGraphMediaRef();

    void*     read();

    void*     loadSceneGraph(char* filename);


    char**    getNameList()       { return nameList; }

    void**    getNodeList()       { return nodeList; }

    void**    getGeometryList()   { return geometryList; }

    void**    getMaterialList()   { return materialList; }

    void**    getTextureList()    { return textureList; }
   

    int       nodeCount;

    char**    nameList;

    void**    nodeList;

    void**    geometryList;

    void**    materialList;

    void**    textureList;
};

#endif /* __MLE_3DSCENEM_H_ */
