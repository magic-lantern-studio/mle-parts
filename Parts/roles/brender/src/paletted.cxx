/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file paletted.cxx
 * @ingroup MleParts
 *
 * This file implements the Palette Role
 * targeting the BRender platform.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2025 Wizzer Works
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


#ifdef _WINDOWS
void MlePaletteRole::loadPalette(MediaRef palmref)
{
    MlePaletteType *oldPalette = palette;

    // Declare local variables.
#ifdef _WINDOWS_RIFF_PALETTE_EXPECTED
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
#ifdef _WINDOWS_RIFF_PALETTE_EXPECTED
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
