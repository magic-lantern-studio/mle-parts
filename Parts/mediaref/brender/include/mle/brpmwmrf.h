/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brpmwmrf.h
 * @ingroup MleParts
 *
 * This file defines the class for a Pixelmap Media Reference
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

#ifndef __MLE_BRPMWMRF_H_
#define __MLE_BRPMWMRF_H_


// Include Runtime Player header files.
#include <mle/MleMediaRef.h>

// Include parent class header file.
#include <mle/brpmmref.h>

/**
 * @brief BRender Pixelmap Web-capable media reference.
 *
 * This class implements a Media Reference loader for BRender pixelmaps.
 */
class brpmwmrf : public brpmmref
{
    MLE_MEDIAREF_HEADER(brpmwmrf);
    
  // These are all we need from the base class.
  public:

    brpmwmrf();
};


#endif /* __MLE_BRPMWMRF_H_ */
