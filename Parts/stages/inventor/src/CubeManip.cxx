/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file CubeManip.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file implements a manipulator class that uses the CubeDragger.
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

// Include system header files.
#include <stdio.h>

// Include Inventor header files.
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoSurroundScale.h>

// Include Magic Lantern Rehearsal Player files.
#include "CubeDragger.h"
#include "CubeManip.h"

SO_NODE_SOURCE(CubeManip);

/////////////////////////////////////////////////////////////////////////////
//
// Initializes the class.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeManip::initClass()
{
    //SO_NODE_INIT_CLASS(CubeManip, SoHideableTransformManip, "HideableTransformManip");
	SO_NODE_INIT_CLASS(CubeManip, SoTransformManip, "HideableTransformManip");
}

/////////////////////////////////////////////////////////////////////////////
//
// Constructor.
//
/////////////////////////////////////////////////////////////////////////////

CubeManip::CubeManip()
{
    SO_NODE_CONSTRUCTOR(CubeManip);

    CubeDragger *d = new CubeDragger;
    setDragger(d);
    SoSurroundScale *ss = (SoSurroundScale *) d->getPart("surroundScale",TRUE);
    ss->numNodesUpToContainer = 4;
    ss->numNodesUpToReset = 3;
}

/////////////////////////////////////////////////////////////////////////////
//
// Destructor.
//
/////////////////////////////////////////////////////////////////////////////

CubeManip::~CubeManip()
{
}
