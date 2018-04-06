/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dgeomrm.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Geometry Registry Media Reference
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

#ifndef __MLE_3DGEOMRM_H_
#define __MLE_3DGEOMRM_H_

// Include Runtime Player header files.
#include "mle/MleMediaRef.h"
#include "mle/brendapi.h"

#include "mle/mlTypes.h"


/**
 * @brief 3d Geometry Registry media reference for BRender.
 */
class Mle3dGeometryRegistryMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dGeometryRegistryMediaRef);
    
  public:

    Mle3dGeometryRegistryMediaRef();

    virtual       ~Mle3dGeometryRegistryMediaRef();

    br_model**    read();

    unsigned int  getSize() { return m_geometryRegistrySize; }

    br_model**    getList() { return m_geometryRegistryList; }

    void	      setList(br_model**m) { m_geometryRegistryList = m; } // XXX might leak

    void	      setSize(unsigned int s) { m_geometryRegistrySize = s; }

    static br_model** loadModelRegistry(char* filename, unsigned int& numModels);
    
  private:

    unsigned int      m_geometryRegistrySize;
    br_model**        m_geometryRegistryList;
};


br_actor*     mlBrLoadModelRegistry(char* filename, char* id);
br_actor*     mlBrGetModelsWithName(char* name, br_model** geometryList, unsigned int numModels, char* id);
br_actor*     mlBrGetAllModels(br_model** geometryList, unsigned int numModels, char* id);
br_actor**    mlBrGetModelGroups(br_model** geometryList, unsigned int numModels, unsigned int& numActors);

br_model*     mlBrLoadModel(char* filename);
br_model*     mlBrLoadModel(char* filename, unsigned int numPaths, char** pathList);


#endif
