/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MultiSearchAction.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file implements a subclass of SoSearchAction that allows searching
 * for more than one type at the same time.
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
