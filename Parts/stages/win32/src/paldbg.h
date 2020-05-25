/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file paldbg.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines a class for debugging a color palette on the
 * Windows platform.
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

#ifndef __MLE_PALDBG_H_
#define __MLE_PALDBG_H_


// Include system header files.
#include <windows.h>

// Include Magic Lantern header files.
#include "mle/mlTypes.h"


class MlePalDbg
{
  // Declare member variables.
  private:

    HWND m_window;        // Handle to window.

    int m_sizeX;          // Height of palette window.
    int m_sizeY;          // Width of palette window.

    int m_numColors;      // Number of device colors.

  // Declare member functions.
  public:

    MlePalDbg(void);

    virtual ~MlePalDbg(void);

    //
    // This method is used to update the display of the palette.
    //
    MlBoolean draw(void);

};


#endif /* __MLE_PALDBG_H_ */
