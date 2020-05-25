/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3danima.cxx
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Camera Actor.
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

// Include Magic Lantern header files.
#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"
#include "math/3dmath.h"
#include "mle/3danima.h"
#include "mle/3dtranrc.h"


MLE_ACTOR_SOURCE(Mle3dAnimationActor,MleActor);

Mle3dAnimationActor::Mle3dAnimationActor()
{
	// This will be done in the frame animation package
    animationPlayMode = TRUE;
    animationSpeed = 1;
    animationFirstFrame = 0;
    animationLastFrame = -1;
    animationNumber = 0;
    animationAutoUpdateFlag = 0;
    animationAutoUpdateParent = 0;
    animationAutoUpdateTransform.makeIdentity();
    
    sceneGraphSize = 0;
    animationTransformList = NULL;
}


#ifdef MLE_REHEARSAL
void Mle3dAnimationActor::initClass()
{
    //Register parent actor's Members
    mleRegisterActorClass(Mle3dAnimationActor, MleActor);
    
    //Expose these Property Members to the tools
    mlRegisterActorMember(Mle3dAnimationActor, nodeType, int);
    mlRegisterActorMember(Mle3dAnimationActor, render.style, int);
    mlRegisterActorMember(Mle3dAnimationActor, render.shading, int);
    mlRegisterActorMember(Mle3dAnimationActor, render.texturing, int);
    mlRegisterActorMember(Mle3dAnimationActor, render.textureBlending, int);
    mlRegisterActorMember(Mle3dAnimationActor, render.boundingBox, int);
    mlRegisterActorMember(Mle3dAnimationActor, render.facesDoubleSided, int);
    mlRegisterActorMember(Mle3dAnimationActor, render.facesAlwaysInFront, int);
    
    mlRegisterActorMember(Mle3dAnimationActor, animationPlayMode, int);
    mlRegisterActorMember(Mle3dAnimationActor, animationSpeed, int);
    mlRegisterActorMember(Mle3dAnimationActor, animationFirstFrame, int);
    mlRegisterActorMember(Mle3dAnimationActor, animationLastFrame, int);
    mlRegisterActorMember(Mle3dAnimationActor, animationNumber, int);
    mlRegisterActorMember(Mle3dAnimationActor, animationAutoUpdateFlag, int);
    mlRegisterActorMember(Mle3dAnimationActor, animationAutoUpdateParent, int);
    mlRegisterActorMember(Mle3dAnimationActor, animationAutoUpdateTransform, MlTransform);
    
    mlRegisterActorMember(Mle3dAnimationActor, position, MlVector3);
    mlRegisterActorMember(Mle3dAnimationActor, orientation, MlRotation);
    mlRegisterActorMember(Mle3dAnimationActor, scale, MlVector3);
    mlRegisterActorMember(Mle3dAnimationActor, sceneGraph, MlMediaRef);
    mlRegisterActorMember(Mle3dAnimationActor, animationRegistry, MlMediaRef);

    //
    // Mark all the properties that belongs to the "transform" property
    // data set.
    //
    mlRegisterActorMemberDataset(Mle3dAnimationActor, position, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle3dAnimationActor, orientation, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle3dAnimationActor, scale, MLE_PROP_DATASET_TRANSFORM);
}


void Mle3dAnimationActor::resolveEdit(const char* property)
{ 
    if (property)
	{
		if ( !strcmp("nodeType",property))
		{
			nodeType.push(this);
		} else if (! strcmp("render.texturing",property))
		{
			resolveEdit("sceneGraph"); //This is needed to update texture flags
			render.push(this);
		} else if (! strncmp("render.",property,7))
		{
			render.push(this);
		} else if (! strcmp("position",property))
		{
			position.push(this);
		} else if (! strcmp("orientation",property))
		{
			orientation.push(this);
		} else if (! strcmp("animationAutoUpdateParent",property))
		{
			if (animationAutoUpdateFlag)
			{
			    animationAutoUpdateTransform=computeAnimationAutoUpdateTransform
			        (animationAutoUpdateParent);
			}
		} else if (! strcmp("animationNumber",property))
		{
			setupAnimation(animationNumber);
		} else if (! strcmp("scale",property))
		{
			scale.push(this);
		} else if (! strcmp("sceneGraph",property) ||
			   ! strcmp("animationRegistry",property))
		{
			if (animation) delete animation;
				theTitle->scheduler->remove(this);
			int lastFrame = frame;
				init(); // Must initialize all media at once
			frame = lastFrame;
		}
	} else
	{
		// Update position properties only
		update();
    }
}
#endif /* MLE_REHEARSAL */


Mle3dAnimationActor::~Mle3dAnimationActor()
{
	// Unschedule the behave() function
    g_theTitle->m_theScheduler->remove(this);
}


MlTransform Mle3dAnimationActor::computeAnimationAutoUpdateTransform(int parent)
{
    MlTransform result;
    
    Mle3dSequence* sequence = Mle3dAnimation::getSequence(animation,(unsigned int)parent);
    
    if (sequence)
	{
		MlTransform initial,final;
		if (animationFirstFrame >= 0)
			  initial = sequence->m_frame[animationFirstFrame];
		else
			  initial.makeIdentity();
		if (animationLastFrame <= animation->m_numFrames)
			  final = sequence->m_frame[animationLastFrame];
		else
			  final.makeIdentity();
		result = initial.inverse()*final;
    }
    return result;
}


int Mle3dAnimationActor::setupAnimation(int newAnimationNumber)
{
    int result = FALSE;
    if (animationRegistry.m_animationRegistry)
	{ 
        if (newAnimationNumber >= animationRegistry.m_animationRegistry->m_numAnimations)
            newAnimationNumber = animationRegistry.m_animationRegistry->m_numAnimations - 1;
        else
            if (newAnimationNumber < 0) 
                newAnimationNumber = 0;
       
        Mle3dAnimation* newAnimation = NULL;
        newAnimation = Mle3dAnimationRegistry::getAnimation(animationRegistry.m_animationRegistry,
		    newAnimationNumber);
        if (newAnimation)
		{ 
            animation = newAnimation;
               
            if ((animationFirstFrame < 0) || (animationFirstFrame >= animation->m_numFrames))
                animationFirstFrame = 0;
   
            if ((animationLastFrame == -1) || (animationLastFrame >= animation->m_numFrames)) 
   	            animationLastFrame = animation->m_numFrames - 1;
	     
            if (animationTransformList) 
                delete [] animationTransformList;

            animationTransformList = new MlTransform*[animation->m_numSequences + 1];
            animationTransformList[animation->m_numSequences] = NULL;
	    
            frame=animationFirstFrame;
	    
            if (animationAutoUpdateFlag)
			{
               animationAutoUpdateTransform = computeAnimationAutoUpdateTransform
                                        (animationAutoUpdateParent);
			}

            result = TRUE;
		}
        else
            newAnimationNumber = animationNumber;
    
        animationNumber = newAnimationNumber;
	}
    else 
        animationNumber=0;
   
    return result;
}


void  Mle3dAnimationActor::init()
{
    // XXX Push this property on init only (for now)
    if (sceneGraphSize = sceneGraph.push(this))
	{
	    animationObjectNameList=sceneGraph.getNameList();
	
	    if ((animationRegistry.push(this)) && setupAnimation(animationNumber))
		{
			g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,behave,this,this,1,1);
	        behave(this); // Apply the animation for the first frame
		}
    }
    update();
}


void Mle3dAnimationActor::update()
{
    // Push the values of the properties that can be set from the workprint
    nodeType.push(this);
    render.push(this);
    position.push(this);
    orientation.push(this);
    scale.push(this);
}


void Mle3dAnimationActor::behave(void* client)
{
    // Do the actor's behavior
    
    Mle3dAnimationActor* a = (Mle3dAnimationActor*)client;
    
    if (a->animationPlayMode)
	{ 
        // Cycle through the available animations in the registry after each one plays all frames
        // Restarts at the first frame at the beginning of each cycle
        if (a->frame>a->animationLastFrame)
		{
            a->animationNumber++;
	        if (a->animationNumber >= a->animationRegistry.m_animationRegistry->m_numAnimations) 
                a->animationNumber=0;
	        a->setupAnimation(a->animationNumber);
    	
	        // Start the actor out in the position it was in at the last frame
	        if (a->animationAutoUpdateFlag)
			{ 
	            MlTransform transform;
	            Mle3dTransformCarrier::get(a->getRole(),transform);
	            transform *= a->animationAutoUpdateTransform;
	            Mle3dTransformCarrier::set(a->getRole(),transform);
			}  
	        a->frame = a->animationFirstFrame;
		}
    
	if ((a->frame>=0) && (a->frame<a->animation->m_numFrames))
	{
	    // Update the transform list to reference the list of transforms from
	    // the current frame of the animation sequence
	    Mle3dSequence* sequence=NULL;
	    for (unsigned int i=0;i<a->sceneGraphSize;i++)
		{
		    if (a->animationObjectNameList && a->animationObjectNameList[i] &&
		       (sequence=Mle3dAnimation::getSequence(a->animation, a->animationObjectNameList[i])))
		        a->animationTransformList[i] = &(sequence->m_frame[a->frame]);
            else
		        a->animationTransformList[i] = NULL;
		}
	    // Apply the updated transforms to the scene graph
	    Mle3dTransformRegistryCarrier::set(a->sceneGraph.getTransformList(),
		    a->animationTransformList);

	    // Increment the frame counter
	    // XXX The frame animation package will eventually take care of this 
	    a->frame += a->animationSpeed;
	}
        else
            a->frame = 0;
	
    }
}

