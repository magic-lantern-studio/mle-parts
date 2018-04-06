/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dscenep.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Scene Graph property.
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

#ifndef __MLE_3DSCENEP_H_
#define __MLE_3DSCENEP_H_

// Include Magic Lanter header files.
#include "mle/mlTypes.h"
#include "mle/3dscenec.h"

class MleActor;

class Mle3dSceneGraphProperty
{
  public:

    MlMediaRef m_sceneGraph;

    Mle3dSceneGraphProperty()        
    {
		m_sceneGraph = MLE_NO_MEDIA; 
        m_nameList = NULL;
        m_nodeList = m_geometryList = m_materialList = m_textureList = m_transformList = NULL;
    }


    ~Mle3dSceneGraphProperty()
    {
		delete[] m_nameList;
        delete[] m_nodeList;
        delete[] m_geometryList;
        delete[] m_materialList;
        delete[] m_textureList;
        delete[] m_transformList;
    }


    MlMediaRef operator=(MlMediaRef newValue)
    { return m_sceneGraph = newValue; }


    operator MlMediaRef&  () 
    { return m_sceneGraph; }


    int push(MleActor* actor)  
    {
		return Mle3dSceneGraphCarrier::set(actor->getRole(),m_sceneGraph,
            m_nameList,m_nodeList,m_geometryList,m_materialList,m_textureList,m_transformList); 
    }


    char**   getNameList()       { return m_nameList; }

    int      getNameCount()      { int count=0; while (m_nameList[count]) count++; return count; }

    void**   getNodeList()       { return m_nodeList; }

    int      getNodeCount()      { int count=0; while (m_nodeList[count]) count++; return count; }

    void**   getGeometryList()   { return m_geometryList; }

    int      getGeometryCount()  { int count=0; while (m_geometryList[count]) count++; return count; }

    void**   getMaterialList()   { return m_materialList; }

    int      getMaterialCount()  { int count=0; while (m_materialList[count]) count++; return count; }

    void**   getTextureList()    { return m_textureList; }

    int      getTextureCount()   { int count=0; while (m_textureList[count]) count++; return count; }

    void**   getTransformList()  { return m_transformList; }

    int      getTransformCount() { int count=0; while (m_transformList[count]) count++; return count; }
  

  private:

    char** m_nameList;
    void** m_nodeList;
    void** m_geometryList;
    void** m_materialList;
    void** m_textureList;
    void** m_transformList;
};

#endif /* __MLE_3DSCENEP_H_ */
