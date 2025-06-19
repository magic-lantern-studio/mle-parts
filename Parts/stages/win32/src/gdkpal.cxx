/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file gdkpal.cxx
 * @ingroup MlParts
 *
 * This file implements a Magic Lantern class for creating a
 * DirectDraw color palette.
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

#ifdef _GDK_

// Include PC BRender target header files.
#include "mle/gdkpal.h"
#include "mle/pcstage.h"


// Create a Gdk (DirectDraw) color palette.
MleGdkPalette::MleGdkPalette(PALETTEENTRY *palEntries)
{
    // Declare local variables.
    PALETTEENTRY pe[MLE_PC_PALETTE_SIZE], *pPtr;
    HRESULT ddrval;

    if (palEntries == NULL)
	{
        defaultPalette(pe, 0, rgbToPaletteEntry(0,0,0));
        pPtr = pe;
    } else
	{
        pPtr = palEntries;
    }

    ddrval = ((PCBRenderStage *)MleStage::g_theStage)->ddObj->CreatePalette(
		DDPCAPS_8BIT, pPtr, &m_pPalette, NULL);
    MLE_ASSERT(ddrval == DD_OK);
}

// Delete a Gdk color palette.
MleGdkPalette::~MleGdkPalette()
{
    // The palette should have been removed from the palette manager's
    // registry prior to deletion.
    // XXX - note to me - have palmgr deletion return the pointer.
    MLE_ASSERT(~(m_flags & MLE_PALETTE_REGISTERED));

    // Release() will decrement the DirectDraw palette's reference count,
    // and free it when the count reaches zero.  It returns the ref count.
    // XXX need a policy as to whether we'll use the reference counts and
    // shouldn't deconstruct the MlePalette while the m_pPalette is still
    // referred to, whether we should keep releasing until it's gone,
    // or what.  Could put an MLE_ASSERT here. 
    m_pPalette->Release();
}

// Set this palette in the hardware.
MlBoolean MleGdkPalette::setSystemPalette(void)
{
    HRESULT ddrval;

    ddrval =  ((PCBRenderStage *)MleStage::g_theStage)->m_front->SetPalette(m_pPalette);
    if (ddrval != DD_OK)
	{
        return FALSE;
    }

    return TRUE;
}

// Get or set entries in color palette.
MlBoolean MleGdkPalette::getEntries(MlPaletteIndex start, MlPaletteIndex n, 
    PALETTEENTRY *palEntries)
{
    HRESULT ddrval;

    MLE_ASSERT(palEntries);

    ddrval = m_pPalette->GetEntries(0, start, n, palEntries);
    if (ddrval != DD_OK)
	{
        return FALSE;
    }

    return TRUE;
}

MlBoolean MleGdkPalette::setEntries(MlPaletteIndex start, MlPaletteIndex n, 
    PALETTEENTRY *palEntries)
{
    HRESULT ddrval;

    MLE_ASSERT(palEntries);

    ddrval = m_pPalette->SetEntries(0, start, n, palEntries);
    if (ddrval != DD_OK)
	{
        return FALSE;
    }

    return TRUE;
}


#if defined(MLE_DEBUG)
void MleGdkPalette::dumpEntries(FILE *fp)
{
    // Declare local variables.
    PALETTEENTRY palEntries[MLE_PC_PALETTE_SIZE];

    fprintf(fp,"Dumping MleGdkPalette entries.\n");

    getEntries(0,MLE_PC_PALETTE_SIZE,palEntries);
    MlePCPalette::dumpEntries(fp,palEntries);
}
#endif /* MLE_DEBUG */

#endif /* _GDK_ */
