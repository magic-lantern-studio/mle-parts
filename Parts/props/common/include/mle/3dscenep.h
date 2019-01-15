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
// The MIT License (MIT)
//
// Copyright (c) 2003-2019 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
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

// Include Carrier support.
#include "mle/3dscenec.h"

class MleActor;

class PROPERTY_API Mle3dSceneGraphProperty
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
