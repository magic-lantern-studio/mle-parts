/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file antirot.h
 * @ingroup MlParts
 *
 * This file defines an Inventor node that undoes any rotation applied
 * in the current transformation matrix during scene graph traversal.
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
