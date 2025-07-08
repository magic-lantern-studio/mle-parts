/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MultiSearchAction.h
 * @ingroup MlParts
 *
 * This file defines a subclass of SoSearchAction that allows searching
 * for more than one type at the same time.
 */

 // COPYRIGHT_BEGIN
 //
 // The MIT License (MIT)
 //
 // Copyright (c) 2017-2025 Wizzer Works
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
