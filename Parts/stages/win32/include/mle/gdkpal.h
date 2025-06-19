/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file gdkpal.h
 * @ingroup MlParts
 *
 * This file defines a Magic Lantern class for a creating a
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

#ifndef __MLE_GDKPAL_H_
#define __MLE_GDKPAL_H_

#ifdef _GDK_

// Include DirectDraw header files.
#include <ddraw.h>

// Inlclude PC target header files.
#include "mle/pcpal.h"

/************************************************************************
CLASS
 MleGdkPalette

 This is Magic Lantern class for a creating a DirectDraw color palette.

KEYWORDS
	Windows,DirectDraw,color,palette

OVERVIEW
 This is Magic Lantern class for a creating a DirectDraw color palette.
    MleGdkPalette provides the basic API for creating and modifying
    DirectDraw palettes, which can then be passed to the stage for
    realization in the hardware.

SEE ALSO
 MlePalette, MlePCPalette, MleGdkPalette
*************************************************************************/
class MleGdkPalette : public MlePCPalette
{
  public:

    // Create a Gdk (DirectDraw) color palette.  The palEntries argument,
    // if any, provides a list of 256 colors and palette flags for 
    // initializing the logical palette.  If palEntries is NULL, the default
	// PC palette will be created.
    MleGdkPalette(PALETTEENTRY *palEntries = NULL);
    
    // Delete a Gdk color palette.
    virtual ~MleGdkPalette(void);
    
    // Set this palette in the hardware.
    virtual MlBoolean setSystemPalette(void);
    
    // Get or set a selection of n entries from the palette, beginning
    // with the specified starting index.
    virtual MlBoolean getEntries(MlPaletteIndex start, MlPaletteIndex n, 
				 PALETTEENTRY *palEntries);
    virtual MlBoolean setEntries(MlPaletteIndex start, MlPaletteIndex n, 
				 PALETTEENTRY *palEntries);

#if defined(MLE_DEBUG)

    void dumpEntries(FILE *fp);
#endif /* MLE_DEBUG */
    
  private:

	// Pointer to DirectDraw color palette.
    LPDIRECTDRAWPALETTE	m_pPalette;
};


#endif /* _GDK_ */

#endif /* __MLE_GDKPAL_H_ */
