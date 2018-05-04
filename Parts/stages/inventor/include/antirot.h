/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file antirot.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines an Inventor node that undoes any rotation applied
 * in the current transformation matrix during scene graph traversal.
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

#ifndef __MLE_ANTIROTATE_H_
#define __MLE_ANTIROTATE_H_

// Include Inventor header files.
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTransformation.h>

/**
 * @brief An Inventor node for undoing a rotation.
 *
 * This node undoes any rotation applied in the current transformation
 * matrix during scene graph traversal.
 */
class MleAntiRotate : public SoTransformation
{
	SO_NODE_HEADER(MleAntiRotate);

  public:

	/**
	 * The default constructor.
	 */
	MleAntiRotate(void);

	void doAction(SoAction *action);

	static void initClass(void);

  protected:

	virtual void callback(SoCallbackAction *action);
	virtual void GLRender(SoGLRenderAction *action);
	virtual void getMatrix(SoGetMatrixAction *action);
	virtual void pick(SoPickAction *action);

  private:

	// The destructor.
	~MleAntiRotate();
};

#endif /* __MLE_ANTIROTATE_H_ */
