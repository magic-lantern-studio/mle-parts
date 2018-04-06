/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file CubeManip.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines a manipulator class that uses the CubeDragger.
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

#ifndef __CUBEMANIP_H_
#define __CUBEMANIP_H_

// Include Inventor header files.
//#include <Inventor/manips/SoHideableTransformManip.h>
#include <Inventor/manips/SoTransformManip.h>

//class CubeManip : public SoHideableTransformManip
class CubeManip : public SoTransformManip
{
    SO_NODE_HEADER(CubeManip);

  public:

    /**
	 * The default constructor.
	 */
    CubeManip();

  SoINTERNAL public:

	/**
	 * Initialize the class.
	 */
    static void initClass();

  protected:

	/**
	 * The destructor.
	 */
    virtual ~CubeManip();
};

#endif /* __CUBEMANIP_H_ */
