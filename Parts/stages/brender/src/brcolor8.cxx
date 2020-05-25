/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brcolor8.cxx
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

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/mlMalloc.h"

// Include BRender target header files.
#include "mle/brcolor8.h"

//
// mlBrMapMaterialTo8bit
//
// Map a BRender material to the correct index base and range for
// 8-bit indexed color.  Colors are matched on a per-component
// basis within plus or minus the requested tolerance.
//
// Returns TRUE if successfully mapped.  Returns FALSE if the
// material's RGB value wasn't found in the mapLookup table.
//
MlBoolean mlBrMapMaterialTo8bit(const MlBrRgbTo8bitMapping *mapLookup, 
	unsigned int mapSize, br_material *material, unsigned int tolerance)
{
    int r = (int)BR_RED(material->colour);
    int g = (int)BR_GRN(material->colour);
    int b = (int)BR_BLU(material->colour);

	for (unsigned int i = 0; i < mapSize; i++)
	{
        if ((abs(r - (int)BR_RED(mapLookup[i].color)) <= tolerance) &&
            (abs(g - (int)BR_GRN(mapLookup[i].color)) <= tolerance) &&
            (abs(b - (int)BR_BLU(mapLookup[i].color)) <= tolerance))
		{
			material->index_base = mapLookup[i].index_base;
			material->index_range = mapLookup[i].index_range;
			return TRUE;
		}
	}

	return FALSE;
}

//
// mlBrMapMaterialTo8bit
//
// Map all registered BRender materials to the correct index bases
// and ranges for 8-bit indexed color.  Any material whose RGB value
// is not found in the mapLookup table is set to point to the first
// color in the table.  Colors are matched on a per-component
// basis within plus or minus the requested tolerance.
//
// Returns TRUE if all materials were successfully mapped, or there were
// no materials requiring mapping.  Returns FALSE if an error occurred.
//
MlBoolean  mlBrMapMaterialRegistryTo8bit(const MlBrRgbTo8bitMapping *mapLookup, 
	unsigned int mapSize, unsigned int tolerance)
{
	if (!mapLookup || !mapSize)
		return FALSE;

	unsigned int numMat = BrMaterialCount("*");
	if (!numMat)
		return TRUE;

	br_material **materials = (br_material **)mlMalloc(sizeof(br_material *)*numMat);
	if (!materials)
		return FALSE;

	unsigned int numFound = BrMaterialFindMany("*", materials, numMat);
    if (!numFound)
	{
        mlFree(materials);
		// Return an error flag here.  Since numMat materials were expected,
		// there must be some problem with the material registry.
        return FALSE;
    }

	MlBoolean foundColor;
	for (unsigned int i = 0; i < numFound; i++)
	{
		foundColor = mlBrMapMaterialTo8bit(mapLookup, mapSize, 
			materials[i], tolerance);
		if (foundColor == FALSE)
		{
			// Default to the first ramp.
			materials[i]->index_base = mapLookup[0].index_base;
			materials[i]->index_range =  mapLookup[0].index_range;
		}
		BrMaterialUpdate(materials[i],BR_MATU_ALL);
	}

    mlFree(materials);
	return TRUE;
}
