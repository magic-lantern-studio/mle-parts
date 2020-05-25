/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file myprop2c.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for two properties
 * targeting the BRender platform.
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

#include "mle/brendapi.h"
#include "mle/br3drole.h"
#include "mle/scalarbr.h"
#include "mle/myprop2c.h"


int MyTwoPropertyCarrier::set(MleRole* role, Property1Type Property1Name,
                              Property2Type Property2Name)
{
	int result = FALSE;

    br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
    if (root)
    {
        //  Replace the "propertyN" member with the name of the scene graph variable N.
        root->property1 = Property1Name;
        root->property2 = Property2Name;
        result = TRUE;
    }

    return result;
}


int MyTwoPropertyCarrier::get(MleRole* role, Property1Type& Property1Name,
                              Property2Type& Property2Name)
{
	int result = FALSE;

    br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
    if (root)
    { 
        // Replace the "propertyN" member with the name of the scene graph variable N.
        PropertyName1 = root->property1;
        PropertyName2 = root->property2;
        result = TRUE;
    }

    return result;
}
