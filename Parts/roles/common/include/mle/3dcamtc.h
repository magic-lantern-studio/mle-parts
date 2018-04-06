/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcamtc.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Camera Type carrier.
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

#ifndef __MLE_3DCAMTC_H_
#define __MLE_3DCAMTC_H_


class MleRole;

/**
 * @brief 3D Camera Type property carrier.
 *
 * This carrier passes a 3D camera type property to a Mle3dRole.
 *
 * @seee Mle3dRole
 */
class Mle3dCameraTypeCarrier
{
  public:

    static int set(MleRole* role, int cameraType);
    static int get(MleRole* role, int& cameraType);
};


#endif /* __MLE_3DCAMTC_H_ */
