/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dmva.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Movie Actor.
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

#ifndef __MLE_2DMVA_H_
#define __MLE_2DMVA_H_

#include "mle/mlTypes.h"
#include "math/vector.h"
#include "mle/MleActor.h"
#include "mle/MleMediaRef.h"

class Mle2dMvActor : public MleActor 
{
    
    MLE_ACTOR_HEADER(Mle2dMvActor);
    
  public:

    Mle2dMvActor(void);

    virtual		~Mle2dMvActor(void);
    
    // Initialize the actor and push initial property values to the role.
    virtual void	init(void);
    
#ifdef MLE_REHEARSAL
    // Tools can change property values directly in actor memory.  This
    // function can be used to propagate the changes to its role
    // to make its state consistent.
    virtual void        resolveEdit(const char *property);
#endif  /* MLE_REHEARSAL */
    
    // Set the current position.
    virtual void        setPosition(MlVector2 &pos);
    
    // Set the current movie.
    virtual void        setMovie(MlMediaRef mv);
    
    // The movie can be toggle on (with TRUE) or off (with FALSE).
    void                setVisible(int state);

    int                 getVisible(void);
    
    //
    // Beginning of properties.

    //          position - contains x and y position, (0, 0) correspond to
    //                     lower left corner.
    //          movie - media reference to the movie
    //          displayState - TRUE for on, FALSE for off


    MlVector2           position;

    MlMediaRef          movie;

    int                 displayState;
};


#endif /* __MLE_2DMVA_H_ */
