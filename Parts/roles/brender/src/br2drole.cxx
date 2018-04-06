/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file br2drole.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for a 2D Role targeting
 * the BRender platfrom.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include BRender target header files.
#include "math/scalarbr.h"
#include "mle/2drole.h"
#include "mle/br2dset.h"


MLE_ROLE_ABSTRACT_SOURCE(Mle2dRole, MleRole);


Mle2dRole::Mle2dRole(MleActor *actor)
  : MleRole(actor),
    m_prev(NULL),
    m_next(NULL)
{
    m_bBoxMin[0] = ML_SCALAR_ZERO;
    m_bBoxMin[1] = ML_SCALAR_ZERO;
    m_bBoxMax[0] = ML_SCALAR(639);
    m_bBoxMax[1] = ML_SCALAR(479);
}


Mle2dRole::~Mle2dRole(void)
{
    Mle2dSet *theSet;

    //
    // Call Set to detach itself from the Set link list.
    //
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
