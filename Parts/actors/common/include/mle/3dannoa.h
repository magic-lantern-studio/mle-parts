/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dannoa.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Annotation Actor.
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

#ifndef __MLE_3DANNOA_H_
#define __MLE_3DANNOA_H_

#include "mle/MleActor.h"
#include "math/vector.h"

/**
 * @brief 3D annotaion Actor.
 *
 * This actor encapsulates the functionality to draw 3D line segments,
 * primarily for the purpose of debugging annotation for behaviors.
 * The interface can handle multiple contexts, where each context is
 * presumed to be collection of related line segments.  It is typical
 * to use an actor's this pointer as the context identifier.
 * 
 * The interface is all static functions, so no direct pointer to
 * the annotation actor is needed.  It is an error to have more than
 * one annotation actor in a title.
 */
class Mle3dAnnotation : public MleActor
{
    MLE_ACTOR_HEADER(Mle3dAnnotation);

  public:

    Mle3dAnnotation(void);

    virtual ~Mle3dAnnotation();

    virtual void init(void);
    
    // clear() removes all geometry associated with the context
    static void clear(void *context);
    
    // drawLine() adds a line segment to a context
    static void drawLine(void *context,
			 const MlVector3& from,const MlVector3& to,
			 float red = 1,float grn = 1,float blu = 1);
    
  private:

    // static pointer to the single instance
	static Mle3dAnnotation *instance;
};

// these just for compactness in the annotated code.  They serve to
// encapsulate the "#ifdef ANNOTATION" that would otherwise appear in
// the code being annotated.

#ifdef ANNOTATION
#define ANN_DRAW_LINE(c,p0,p1,r,g,b) \
	{Mle3dAnnotation::drawLine(c,p0,p1,r,g,b);}
#else
#define ANN_DRAW_LINE(c,p0,p1,r,g,b)
#endif

#ifdef ANNOTATION
#define ANN_CLEAR(c) \
	{Mle3dAnnotation::clear(c);}
#else
#define ANN_CLEAR(c)
#endif

#endif /* __MLE_3DANNOA_H_ */
