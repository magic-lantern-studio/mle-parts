/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MultiSearchAction.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 *
 * This file implements a subclass of SoSearchAction that allows searching
 * for more than one type at the same time.
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

// Include Inventor header files.
//#include <Inventor/manips/SoTransformer2Manip.h>
#include <Inventor/manips/SoTransformerManip.h>

// Include Magic Lantern Rehearsal Player header files.
#include "CubeManip.h"
#include "MultiSearchAction.h"

SO_ACTION_SOURCE(MultiSearchAction);

/////////////////////////////////////////////////////////////////////////////
//
// Initializes the MultiSearchAction class.
//
/////////////////////////////////////////////////////////////////////////////

void
MultiSearchAction::initClass()
{
    SO_ACTION_INIT_CLASS(MultiSearchAction, SoSearchAction);

    // Override the default SoNode method to use multi-search
    SO_ACTION_ADD_METHOD(SoNode,    doMultiSearch);

    // Do NOT search manips or under manips
    SO_ACTION_ADD_METHOD(CubeManip,  emptyAction);
    //SO_ACTION_ADD_METHOD(SoTransformer2Manip, emptyAction);
	SO_ACTION_ADD_METHOD(SoTransformerManip, emptyAction);
}

/////////////////////////////////////////////////////////////////////////////
//
// Constructor.
//
/////////////////////////////////////////////////////////////////////////////

MultiSearchAction::MultiSearchAction()
{
    SO_ACTION_CONSTRUCTOR(MultiSearchAction);
}

/////////////////////////////////////////////////////////////////////////////
//
// Destructor.
//
/////////////////////////////////////////////////////////////////////////////

MultiSearchAction::~MultiSearchAction()
{
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets list of types to search for.
//
/////////////////////////////////////////////////////////////////////////////

void
MultiSearchAction::setTypeList(const SoTypeList &list, SbBool derivedIsOk)
{
    types = list;
    derivedOK2 = derivedIsOk;
}

/////////////////////////////////////////////////////////////////////////////
//
// Initiates action on graph.
//
/////////////////////////////////////////////////////////////////////////////

void
MultiSearchAction::beginTraversal(SoNode *node)
{
    // Set up the regular search action so it will never find
    // anything. (Look for a node pointer that has the same value as
    // "this".) This allows us to call the regular search method for
    // groups without worrying about finding the same node in our
    // method and the regular one.
    setNode((SoNode *) this);

    // Do the standard traversal
    SoSearchAction::beginTraversal(node);
}

/////////////////////////////////////////////////////////////////////////////
//
// Method that is used to traverse any node.
//
/////////////////////////////////////////////////////////////////////////////

void
MultiSearchAction::doMultiSearch(SoAction *action, SoNode *node)
{
    // Check this node
    MultiSearchAction *ma = (MultiSearchAction *) action;
    
    SbBool nodeIsMatch = FALSE;

    const SoType	&nodeType = node->getTypeId();
    const SoTypeList	&types = ma->types;

    if (ma->derivedOK2) {
	for (int i = 0; i < types.getLength(); i++) {
	    if (nodeType.isDerivedFrom(types[i])) {
		nodeIsMatch = TRUE;
		break;
	    }
	}
    }
    else {
	for (int i = 0; i < types.getLength(); i++) {
	    if (nodeType == types[i]) {
		nodeIsMatch = TRUE;
		break;
	    }
	}
    }

    if (nodeIsMatch) {
	if (ma->getInterest() == SoSearchAction::ALL)
	    ma->getPaths().append(ma->getCurPath()->copy());

	else {
	    ma->addPath(ma->getCurPath()->copy());
	    if (ma->getInterest() == SoSearchAction::FIRST)
		ma->setFound();
	}
    }

    // Call the regular search method. We know the standard search
    // criterion will fail, since we asked for a bad node. So this
    // will handle just recursion.
    node->search((SoSearchAction *) action);
}

/////////////////////////////////////////////////////////////////////////////
//
// Action that does nothing.
//
/////////////////////////////////////////////////////////////////////////////

void
MultiSearchAction::emptyAction(SoAction *, SoNode *)
{
}
