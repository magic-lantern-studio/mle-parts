/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv2dRole.cxx
 * @ingroup MleParts
 *
 * This file implements the 2D Role targeting the Inventor platform.
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

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/mlMalloc.h"

// Include Inventor Player header files.
#include "mle/iv2dset.h"

// Include Magic Lantern parts header files.
#include "mle/2drole.h"

MLE_ROLE_ABSTRACT_SOURCE(Mle2dRole, MleRole);


Mle2dRole::Mle2dRole(MleActor *actor)
 : MleRole(actor), m_prev(NULL), m_next(NULL)
{
    m_bBoxMin[0] = ML_SCALAR_ZERO;
    m_bBoxMin[1] = ML_SCALAR_ZERO;
    m_bBoxMax[0] = ML_SCALAR(639);
    m_bBoxMax[1] = ML_SCALAR(479);
}


Mle2dRole::~Mle2dRole(void)
{
    Mle2dSet *theSet;

    // Call set to detach itself from the set link list.
    theSet = Mle2dSet::cast(m_set);
    theSet->detach(this);
}


void
Mle2dRole::getBounds(MlScalar min[2], MlScalar max[2])
{
    min[0] = m_bBoxMin[0];
    min[1] = m_bBoxMin[1];
    max[0] = m_bBoxMax[0];
    max[1] = m_bBoxMax[1];
}


void
Mle2dRole::setBounds(MlScalar min[2], MlScalar max[2])
{
    m_bBoxMin[0] = min[0];
    m_bBoxMin[1] = min[1];
    m_bBoxMax[0] = max[0];
    m_bBoxMax[1] = max[1];
}

void*
Mle2dRole::operator new(size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void*
Mle2dRole::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
Mle2dRole::operator delete(void* p)
{
    mlFree(p);
}

void
Mle2dRole::operator delete[](void* p)
{
    mlFree(p);
}
