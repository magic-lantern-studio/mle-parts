/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file palettea.cxx
 * @ingroup MleParts
 *
 * This file defines the base class for a Magic Lantern Palette.
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
#include "mle/palettea.h"
#include "mle/paletted.h"


MLE_ACTOR_SOURCE(MlePaletteActor,MleActor);


MlePaletteActor::MlePaletteActor()
{ 
}

#ifdef MLE_REHEARSAL
void MlePaletteActor::initClass()
{
    // Register parent actor's Members
    mlRegisterActorClass(MlePaletteActor,MleActor);

    // Register the Property Members
    mlRegisterActorMember(MlePaletteActor,colorMap,MlMediaRef);
}
#endif /* MLE_REHEARSAL */


MlePaletteActor::~MlePaletteActor()
{
}


void MlePaletteActor::init()
{
    MlePaletteDelegate *role = MlePaletteDelegate::cast(getRole());

    role->loadPalette(colorMap);
}
