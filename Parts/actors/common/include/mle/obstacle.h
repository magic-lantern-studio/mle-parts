/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file obstacle.h
 * @ingroup MleParts
 *
 * This file defines the class for an Obstacle Actor.
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

#ifndef __MLE_OBSTACLE_H_
#define __MLE_OBSTACLE_H_

#include "mle/MleActor.h"
#include "mle/3dtranlp.h"
#include "mle/3dqrotp.h"
#include "mle/3duscalp.h"
#include "mle/3dmodelp.h"
#include "mle/3dcomapp.h"
#include "mle/3dtexmpp.h"
#include "mle/sphobs.h"

/**
 * @brief Magic Lantern Obstacle Actor
 *
 * Sample 3d obstacle actor.
 *
 * This actor represents a simple spherical obstacle for use with
 * vehicle-based actors which have obstacle avoidance (e.g. MlePursuer)
 * or collision detection (e.g. MleWanderer).
 *
 * @see SphericalObstacle, AvoidSphericalObstacle, Vehicle
 */
class MleObstacle : public MleActor
{
    MLE_ACTOR_HEADER(MleObstacle);
    
  public:

    // Constructor.
    MleObstacle();

    // Destructor.
    ~MleObstacle();

    // Initialize actor (after constructor, before title runs).
    void init();

    // Push actor state into delegate.
    void update();
    
#ifdef MLE_REHEARSAL
    // Update actor after manipulation in tools.
    void resolveEdit(const char *);
#endif
    
    Mle3dTranslationProperty position;

    Mle3dQuaternionRotationProperty orientation;

    Mle3dModelProperty model;

    Mle3dTextureMapProperty textureMap;

    Mle3dColorMapProperty colorMap;

    Mle3dUniformScaleProperty scale;
    
    // Spherical obstacle package, declares this actor to be a
    // spherical obstacle for the purposes of obstacle avoidance and
    // collision detection.
    SphericalObstacle so;
};


#endif /* __MLE_OBSTACLE_H_ */



