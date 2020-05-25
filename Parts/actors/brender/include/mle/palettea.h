/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file palettea.h
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

#ifndef __MLE_PALETTEA_H_
#define __MLE_PALETTEA_H_


// Include Magic Lantern header files
#include "mle/MleActor.h"
#include "mle/MleMediaRef.h"


/**
 * @brief This is an actor that loads and sets a Windows color palette from a
 * mediaref.
 * 
 * MleWin32PaletteMediaRef,MlePaletteDelegate
 */
class MlePaletteActor : public MleActor
{
    MLE_ACTOR_HEADER(MlePaletteActor);

  public:

	  // Create a palette actor.
      MlePaletteActor();

	  // Delete a palette actor.
      virtual ~MlePaletteActor();

      // Initialize a palette actor.
      void init(void);

	  // MediaRef pointing to the palette data.
      MlMediaRef colorMap;
};


#endif /* __MLE_PALETTEA_H_ */
