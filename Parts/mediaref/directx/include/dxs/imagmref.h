/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file imagmref.h
 * @ingroup MleParts
 *
 * This file defines the class for an Image Media Reference
 * targeting the DirectX platform.
 *
 * @author Mark S. Millard
 * @date Nov 16, 2007
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

#ifndef __MLE_IMAGMREF_H_
#define __MLE_IMAGMREF_H_

// Include system header files
#include <d3d9.h>

// Include runtime player header files.
#include "mle/MleMediaRef.h"
#include "mle/mlTypes.h"

// Include DirectX Stage header fiels.
#include "dxs/DxsSurface.h"


/**
 * @brief This class implements a Media Reference loader for a still image.
 */
class MleImageMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(MleImageMediaRef);
    
  public:

	/**
	 * The default constructor.
	 */
    MleImageMediaRef(void);

	/**
	 * The destructor.
	 */
    virtual ~MleImageMediaRef(void);

	/**
	 * @brief Read the image into the supplied surface.
	 *
	 * @param image A pointer to a Magic Lantern DirectX surface container.
	 *
	 * @return <b>TRUE</b> is returned if the image is successfully read.
	 * Otherwise <b>FALSE</b> is returned.
	 */
    MlBoolean read(LPDXS_SURFACE image);
};


#endif /* __MLE_IMAGMREF_H_ */
