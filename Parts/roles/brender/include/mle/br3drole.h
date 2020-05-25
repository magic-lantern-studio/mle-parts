/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file br3drole.h
 * @ingroup MleParts
 *
 * This file contains the class for a 3D Role targeting the BRender
 * platform.
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

#ifndef __MLE_BR3DROLE_H_
#define __MLE_BR3DROLE_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleRole.h"

// Include BRender target header files.
#include "mle/3dnodetp.h"


struct br_actor;


class Mle3dRole : public MleRole
{
    MLE_ROLE_HEADER(Mle3dRole);

  public:

    Mle3dRole(MleActor *actor, int nodeType = Mle3dNodeTypeProperty::TRANSFORM);

    ~Mle3dRole();
    
    virtual void addChild(Mle3dRole *child);

    int setNodeType(int nodeType);

    br_actor *getRoot(void) const { return m_root; }
    
    br_actor *m_root;
};

#endif /* __MLE_BR3DROLE_H_ */
