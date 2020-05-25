/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MultiSearchAction.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines a subclass of SoSearchAction that allows searching
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

#ifndef  __MULTISEARCHACTION_H_
#define  __MULTISEARCHACTION_H_

#include <Inventor/actions/SoSearchAction.h>

class MultiSearchAction : public SoSearchAction
{

    SO_ACTION_HEADER(MultiSearchAction);

  public:

    // Constructor and destructor
    MultiSearchAction();
    virtual ~MultiSearchAction();

    // Initializes the class
    static void	initClass();

    // Sets list of types to search for
    void	setTypeList(const SoTypeList &list, SbBool derivedIsOk = TRUE);

  protected:

    // Initiates action on graph
    virtual void	beginTraversal(SoNode *node);

  private:

    SoTypeList	types;
    SbBool	derivedOK2;

    // Method that is used to traverse any node
    static void	doMultiSearch(SoAction *action, SoNode *node);

    // Action that does nothing
    static void emptyAction(SoAction *, SoNode *);
};

#endif /* __MULTISEARCHACTION_H_ */
