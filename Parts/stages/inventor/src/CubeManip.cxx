/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file CubeManip.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
  *
 * This file implements a manipulator class that uses the CubeDragger.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Wizzer Works
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
