/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dRole.h
 * @ingroup MleParts
 *
 * This file contains the class for a 3D Role targeting the Inventor
 * platfrom.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2003-2021 Wizzer Works
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

#ifndef __MLE_IV3DROLE_H_
#define __MLE_IV3DROLE_H_

// Include Inventor header files. */
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Player header files.
#include "mle/MleRole.h"

// Include Magic Lantern parts header files.
#include "mle/3dnodetp.h"
#include "mle/Mle3dRole.h"


#if defined(MLE_REHEARSAL)
#include "mle/DwpDict.h"
#endif /* MLE_REHEARSAL */

class SoSeparator;


class MLE_3DROLE_API Mle3dRole : public MleRole
{
    MLE_ROLE_HEADER(Mle3dRole);

  public:

    Mle3dRole(MleActor *actor, 
		int nodeType = Mle3dNodeTypeProperty::GEOMETRY);

    ~Mle3dRole();
    
    virtual void addChild(Mle3dRole *child);

    // XXX - should this be a private API?
    int setNodeType(int nodeType);

    SoSeparator *getRoot(void) const { return m_root; }
    
#if defined(MLE_REHEARSAL)

    // Our inventor parent, set by Set, used for deleting our
    // scenegraph on destruction.
    SoGroup *m_ivParent;

    void setIvParent(SoGroup *p) { m_ivParent = p; }

    // Pick registry.
    //   This dictionary is maintained at rehearsal time for roles
    //   to enable picking on themselves.  It is indexed by an Inventor
    //   node pointer and returns a role pointer.
    static MleDwpDict g_pickRegistry;

#endif /* MLE_REHEARSAL */

    /**
     * Override operator new.
     *
     * @param tSize The size, in bytes, to allocate.
     */
    void* operator new(size_t tSize);

    /**
     * Override operator new array.
     *
     * @param tSize The size, in bytes, to allocate.
     */
    void* operator new[](size_t tSize);

    /**
     * Override operator delete.
     *
     * @param p A pointer to the memory to delete.
     */
    void operator delete(void* p);

    /**
     * Override operator delete array.
     *
     * @param p A pointer to the memory to delete.
     */
    void operator delete[](void* p);

  protected:

	SoSeparator *m_root;
};

#endif /* __MLE_IV3DROLE_H_ */
