/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file CubeManip.h
 * @ingroup MlParts
 *
 * This file defines a manipulator class that uses the CubeDragger.
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
