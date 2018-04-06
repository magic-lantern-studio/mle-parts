/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3drendp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Render property.
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

#ifndef __MLE_3DRENDP_H_
#define __MLE_3DRENDP_H_

// Include Magic Lantern header files.
#include "mle/3drendc.h"

class MleActor;

class Mle3dRenderProperty
{
  public:

      int m_style;

      int m_shading;

      int m_texturing;

      int m_textureBlending;

      int m_boundingBox;

      int m_facesDoubleSided;

      int m_facesAlwaysInFront;


    Mle3dRenderProperty()
	{
	  m_style = Mle3dRenderCarrier::FACES; 
      m_shading = TRUE;
      m_texturing = TRUE;
      m_textureBlending = FALSE;
      m_boundingBox = FALSE;
      m_facesDoubleSided = TRUE;
      m_facesAlwaysInFront = FALSE;
	}

    int push(MleActor* actor)
	{
	  return Mle3dRenderCarrier::set(actor->getRole(),
          m_style,m_shading,m_texturing,m_textureBlending,m_boundingBox,
          m_facesDoubleSided,m_facesAlwaysInFront); 
	}

    int pull(MleActor* actor) 
	{
	  return Mle3dRenderCarrier::get(actor->getRole(),
          m_style,m_shading,m_texturing,m_textureBlending,m_boundingBox,
          m_facesDoubleSided,m_facesAlwaysInFront); 
	}
    
};

#endif /* __MLE_3DRENDP_H_ */
