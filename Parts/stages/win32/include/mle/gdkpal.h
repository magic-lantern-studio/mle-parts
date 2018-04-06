/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file gdkpal.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines a Magic Lantern class for a creating a
 * DirectDraw color palette.
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
