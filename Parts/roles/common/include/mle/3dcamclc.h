/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcamclc.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Camera Clipping carrier.
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

#ifndef __MLE_3DCAMCLC_H_
#define __MLE_3DCAMCLC_H_


class MleRole;
class MlVector2;

/**
 * @brief 3D Camera Clipping property carrier.
 *
 * This carrier passes a 3D camera clipping property to a Mle3dRole.
 *
 * @see Mle3dRole
 */
class Mle3dCameraClippingCarrier
{
  public:

    static int set(MleRole* role, MlVector2 clipping);
    static int get(MleRole* role, MlVector2& clipping);
};


#endif /* __MLE_3DCAMCLC_H_ */
