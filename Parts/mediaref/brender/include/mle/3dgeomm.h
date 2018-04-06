/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dgeomm.h
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

#ifndef __MLE_3DGEOMM_H_
#define __MLE_3DGEOMM_H_


// Include Runtime Player header files.
#include "mle/MleMediaRef.h"
#ifdef WIN32
#include "brender.h"
#else
#include "mle/brendapi.h"
#endif /* WIN32 */


#include "mle/mlTypes.h"


class Mle3dGeometryMedia : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dGeometryMedia);

  public:

    Mle3dGeometryMedia();
    virtual           ~Mle3dGeometryMedia(void);
    br_model**        read(void);
    unsigned int      getGeometryCount() { return m_geometryCount; }
    br_model**        getGeometryList() { return m_geometryList; }
    static br_model** loadModelRegistry(char* filename, unsigned int& numModels);

  private:

    unsigned int      m_geometryCount;
    br_model**        m_geometryList;
};


br_actor*     mlBrLoadModelRegistry(char* filename, char* id);
br_actor*     mlBrGetModelsWithName(char* name, br_model** geometryList, unsigned int numModels, char* id);
br_actor*     mlBrGetAllModels(br_model** geometryList, unsigned int numModels, char* id);
br_actor**    mlBrGetModelGroups(br_model** geometryList, unsigned int numModels, unsigned int& numActors);

br_model*     mlBrLoadModel(char* filename);
br_model*     mlBrLoadModel(char* filename, unsigned int numPaths, char** pathList);


#endif /* __MLE_3DGEOMM_H_*/
