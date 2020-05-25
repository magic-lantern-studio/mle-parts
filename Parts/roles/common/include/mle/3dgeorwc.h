/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3danimrc.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Animation Registry carrier.
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

#ifndef __MLE_3DGEORWC_H_
#define __MLE_3DGEORWC_H_

#include "mle/MleMediaRef.h"
#include "mle/3dgeomrc.h"

/**
 * @brief 3D Geometry Registry Web property carrier.
 *
 * This carrier uses Web I/O to download 3D geometry registry media.  The
 * carrier does this by setting the media ref, which in turn invokes the
 * networked I/O.  
 * 
 * The main difference between this carrier and the
 * Mle3dGeometryRegistryCarrier is that this carrier is able to accomodate
 * asynchronous media ref I/O, while the latter requires synchronous
 * loading of the media to be completed during the set() method call.
 * This is done by posting a callback with the media ref loader, so that
 * this carrier can finish its work in the callback after media are loaded.
 * 
 * @see Mle3dRole, Mle3dGeometryRegistryWebMediaRef, MleReferenceConverter
 * @see MleReferenceUrlConverter
 */
class Mle3dGeometryRegistryWebCarrier : public Mle3dGeometryRegistryCarrier 
{
  public:
    //XXX This function ideally should use MlTransform* as return type
    //XXX Appears to inherit from parent without async I/O changes.

    static void** set(MleRole* role, MlMediaRef geometryRegistry);

    /**
	 * Pushes role and media ref information to the carrier, with
	 * the result that the asynchronous Web downloading of the media
	 * begins.  The set() method also posts a callback for execution
	 * on completion of the media downloading, so that the media can
	 * be added to the role's scene graph at that time.
	 * 
	 * This method overrides the method inherited from 
	 * Mle3dGeometryRegistryCarrier.
	 */
    static void** set(MleRole* role, MlMediaRef geometryRegistry,
	    int& geometryRegistrySize);

    /**
     * The callback to be invoked once asynchronous Web downloading is
     * complete.  This method adds the geometry registry media to the
     * actor's role, replacing a previous geometry registry if necessary.
     */
    static void apply( MlMediaRef* geometryMedia, MleRole* del );
};


#endif /* __MLE_3DGEORWD_H_ */
