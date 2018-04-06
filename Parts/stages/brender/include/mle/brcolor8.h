/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brcolor8.h
 * @ingroup MleParts
 *
 * This file provides utilities for mapping RGB BRender colors to index
 * base and range for stages with 8-bit indexed color.
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

#ifndef __MLE_BRCOLOR8_H_
#define __MLE_BRCOLOR8_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include BRender target header files.
#include "math/scalarbr.h"
#include "mle/brendapi.h"


// MlBrRgbTo8bitMapping is a structure used to build tables mapping 
// specific RGB values to their ramps in a particular color 
// palette.  The color field contains the RGB values packed into
// a BRender br_colour word.  The index_base is the starting index
// of the color's contiguous ramp in the palette.  Adding index_range
// + index_base gives the ending index of the color's ramp.
//
typedef struct _MlBrRgbTo8bitMapping
{
    br_colour   color;
    br_uint_8   index_base;
    br_uint_8   index_range;
} MlBrRgbTo8bitMapping;


// mlBrMapMaterialTo8bit() sets a BRender material's index_base and 
// index_range to the values appropriate for its RGB colour value.
// Colors are matched on a per-component basis within plus or minus 
// the requested tolerance.
//
// It returns TRUE if it successfully mapped the RGB value to its
// 8-bit indexed values, and FALSE if the material's RGB value wasn't 
// found in the mapLookup table.
//
MlBoolean mlBrMapMaterialTo8bit(const MlBrRgbTo8bitMapping *mapLookup, 
	unsigned int mapSize, br_material *material, unsigned int tolerance = 0);


// mlBrMapMaterialTo8bit() maps all currently registered BRender materials 
// to the correct index bases and ranges for 8-bit indexed color.  Any 
// material whose RGB value is not found in the mapLookup table is set 
// to point to the first color in the table.  Colors are matched on a 
// per-component basis within plus or minus the requested tolerance.
//
// It returns TRUE if all materials were successfully mapped, or there were
// no materials requiring mapping, and FALSE if an error occurred.
//
MlBoolean mlBrMapMaterialRegistryTo8bit(const MlBrRgbTo8bitMapping *mapLookup,
	unsigned int mapSize, unsigned int tolerance = 0);


#endif /* __MLE_BRCOLOR8_H_ */
