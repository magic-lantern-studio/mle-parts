/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brpalmrf.h
 * @ingroup MleParts
 *
 * This file defines the class for a Palette Media Reference
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

#ifndef __MLE_BRPALMRF_H_
#define __MLE_BRPALMRF_H_

//
//  Class definition of Magic Lantern Win32/GDK BRender Palette MediaRef 
//  object, MleBrPaletteMediaRef.
//

// Include Runtime Player header files.
#include "mle/MleMediaRef.h"
#include "mle/brendapi.h"
#include "mle/pcpal.h"

/**
 * @brief BRender Palette media reference.
 *
 * A Media Reference loader for BRender pixelmap color palettes.
 *
 * @see MlePCPalette,MleMediaRef
 */
class MleBrPaletteMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(MleBrPaletteMediaRef);
    
  public:

	// Create a palette mediaref.
    MleBrPaletteMediaRef(void);


	// Delete a palette mediaref.
    virtual ~MleBrPaletteMediaRef(void);


	// Read the BRender pixelmap containing the palette.
    br_pixelmap *readBrPalette(void);


    // Load the palette media and set the MlePCPalette for it.
    // The MlePCPalette should either be a MleWin32Palette or
    // a MleGdkPalette.
    MlBoolean read(MlePCPalette *palette);


    // Load the mediaref palette data into a PALETTEENTRY table.
    PALETTEENTRY *read(void);
};


#endif /* __MLE_BRPALMRF_H_ */
