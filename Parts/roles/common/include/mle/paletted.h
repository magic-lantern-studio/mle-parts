/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file paletted.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D palette property.
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

#ifndef __MLE_PALETTED_H_
#define __MLE_PALETTED_H_

// In order to bind the palette actor to the palette role, we need
// a set.  Presently we're using the 2d set, and so we have to have dummy
// getBounds, setBounds, and draw methods.
#include <limits.h>

// Include Magic Lantern header files.
#include "mle/2drole.h"
#include "mle/brpalmrf.h"
#ifdef WIN32
	// XXX currently only implemented for the PC stage.
#include "mle/pcstage.h"
#endif


/**
 * @brief This is a role that loads and sets a Windows color palette from a 
 * mediaref.
 *
 * @see MleWin32PaletteMediaRef,MlePaletteActor
 */
class MlePaletteRole : public Mle2dRole
{
    MLE_ROLE_HEADER(MlePaletteRole);

  public:
	// Create a palette role.
    MlePaletteRole(MleActor *actor);

	// Delete a palette role.
    virtual ~MlePaletteRole();

#ifdef WIN32
	// Load palette from mediaref.
    void loadPalette(MlMediaRef palmref);

	// Pointer to palette object.
    MlePaletteType *palette;
#else
	// Pointer to palette object (non-Windows). Not currently used.
	void *palette;
#endif

	// Dummy bounding box get routine (needed because MlePaletteRole
	// is derived from Mle2dRole. Returns a harmless bounding box.
    void        getBounds(MlScalar min[2], MlScalar max[2])
	{
	    min[0] = ML_SCALAR(INT_MAX);
		min[1] = ML_SCALAR(INT_MAX);
		max[0] = ML_SCALAR(INT_MIN);
		max[1] = ML_SCALAR(INT_MIN);
	}

	// Dummy bounding box set routine (needed because MlePaletteRole
	// is derived from Mle2dRole. Does nothing.
    void        setBounds(MlScalar min[2], MlScalar max[2]) {}

	// Dummy draw routine (needed because MlePaletteRole
	// is derived from Mle2dRole. Does nothing.
    void        draw(void *data) {}

};

#endif /* __MLE_PALETTED_H_ */
