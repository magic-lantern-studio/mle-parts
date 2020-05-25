/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtexrwm.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Texture Registry Media Reference
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

#include <mle/MleMediaRef.h>
#include <mle/refucvt.h>
#include <mle/3dtexrwm.h>

MLE_MEDIAREF_SOURCE(Mle3dTextureRegistryWebMediaRef,Mle3dTextureRegistryMediaRef);

Mle3dTextureRegistryWebMediaRef::Mle3dTextureRegistryWebMediaRef()
{
	// Replace the old ref converter with one that understands URLs
	delRefConverter();
	setRefConverter(new MleReferenceUrlConverter);
}
