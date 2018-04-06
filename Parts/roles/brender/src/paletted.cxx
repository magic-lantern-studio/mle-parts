/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file paletted.cxx
 * @ingroup MleParts
 *
 * This file implements the Palette Role
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

// Include Magic Lantern header files.
#include "mle/MleLoad.h"

// Include Role header files.
#include "mle/palettea.h"
#include "mle/paletted.h"


MLE_ROLE_SOURCE(MlePaletteRole,Mle2dRole);


MlePaletteRole::MlePaletteRole(MleActor *actor)
    :Mle2dRole(actor)
{ 
	palette = NULL;
}

MlePaletteRole::~MlePaletteRole()
{
	delete palette;
}


#ifdef WIN32
void MlePaletteRole::loadPalette(MediaRef palmref)
{
	MlePaletteType *oldPalette = palette;

    // Declare local variables.
#ifdef WIN32_RIFF_PALETTE_EXPECTED
	// XXX the RIFF .PAL mediaref isn't working yet.
    MleWin32PaletteMediaRef *paletteMediaRef;
#else
	// XXX meanwhile, using a BRender pixelmap palette.
	// eventually, this would be a property so that
	// either format could be used.
    MleBrPaletteMediaRef *paletteMediaRef;
#endif
    MlePCBRenderStage *stage;
    PALETTEENTRY *paletteEntries;

    // Load palette from MediaRef.
#ifdef WIN32_RIFF_PALETTE_EXPECTED
    paletteMediaRef = (MleWin32PaletteMediaRef *) mlLoadMediaRef(palmref,NULL);
#else
    paletteMediaRef = (MleBrPaletteMediaRef *) mlLoadMediaRef(palmref,NULL);
#endif
    paletteEntries = paletteMediaRef->read();
	if (paletteEntries)
	{
#ifdef _GDK_
		palette = new MlePaletteType(paletteEntries);
#else
		palette = new MlePaletteType(paletteEntries, 
			mlWin32PalConfig_236Colors, MLE_PC_PALETTE_SIZE);
#endif
	}
	if (palette)
	{
		// Set/realize palette.
		stage = (MlePCBRenderStage *)MleStage::theStage;
		stage->setPalette(palette);
		delete oldPalette;
	}
	else
		palette = oldPalette;
}
#endif
