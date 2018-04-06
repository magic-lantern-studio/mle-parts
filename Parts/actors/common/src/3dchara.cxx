/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dchara.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Character Animation Actor.
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

#include "mle/MleScheduler.h"
#include "mle/MleKeyboardPolled.h"
#include "mle/MleDirector.h"
#include "math/3dmath.h"
#include "mle/3dtranrc.h"
#include "mle/3dchara.h"


MLE_ACTOR_SOURCE(Mle3dCharacterActor,Mle3dAnimationActor);

Mle3dCharacterActor::Mle3dCharacterActor()
{ 
    character = NULL;
    characterNumber = 0;
    snippet = NULL;
    snippetNumber = 0;
}


#ifdef MLE_REHEARSAL
void Mle3dCharacterActor::initClass()
{
    // Register parent actor's Members
    mlRegisterActorClass(Mle3dCharacterActor, Mle3dAnimationActor);
    
    // Expose these Property Members to the tools
    mlRegisterActorMember(Mle3dCharacterActor, characterRegistry, MediaRef);
    mlRegisterActorMember(Mle3dCharacterActor, characterNumber, int);
    mlRegisterActorMember(Mle3dCharacterActor, snippetNumber, int);
}


void Mle3dCharacterActor::resolveEdit(const char* property)
{ 
    if (property) 
	{
        Mle3dAnimationActor::resolveEdit(property);
        if (! strcmp("characterRegistry",property))
            characterRegistry.push(this);

        if ((!strcmp("characterNumber",property)) ||
            (!strcmp("snippetNumber",property)) ||
            (!strcmp("characterRegistry",property)))
            if (characterRegistry.characterRegistry &&
                (characterNumber<characterRegistry.characterRegistry->numCharacters))
			{
                character = characterRegistry.characterRegistry->character[characterNumber];
                if (character && (snippetNumber<character->numSnippets))
				{
					snippet=character->snippet[snippetNumber];
                    g_theTitle->m_theScheduler->remove(this);
                    g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,behave,this,this,1,1);
				}
                else
				{
					snippetNumber=-1;
                    g_theTitle->m_theScheduler->remove(this);
                    g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,Mle3dAnimationActor::behave,this,this,1,1);
				}
			}
            else
                characterNumber = 0;
	} 
    else 
	{
        //update position properties only
        update();
	}
}
#endif /* MLE_REHEARSAL */


Mle3dCharacterActor::~Mle3dCharacterActor()
{
	// Unschedule the behave() function
    g_theTitle->m_theScheduler->remove(this);
    // delete MleKeyboardPolled::keyboardManager;
}



void  Mle3dCharacterActor::init()
{
    Mle3dAnimationActor::init();

    if (MleKeyboardPolled::g_keyboardManager == NULL)
       new MleKeyboardPolled;

    characterRegistry.push(this);
    if (characterRegistry.m_characterRegistry &&
        (characterNumber<characterRegistry.m_characterRegistry->m_numCharacters))
	{
        character = characterRegistry.m_characterRegistry->m_character[characterNumber];
        if (character && (snippetNumber<character->m_numSnippets))
		{
			snippet=character->m_snippet[snippetNumber];
            g_theTitle->m_theScheduler->remove(this);
            g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,behave,this,this,1,1);
		}
	}
}


void Mle3dCharacterActor::update()
{
    Mle3dAnimationActor::update();
}


void Mle3dCharacterActor::checkForEvents()
{
    if (MleKeyboardPolled::g_keyboardManager->keyDown(MLE_KEY_q))
        g_theTitle->m_quit = TRUE;

    // Check all snippet transitions for relevant keyboard events
    if (snippet)
        for (int i = 0; i < snippet->m_numTransitions; i++)
		{
            int event = snippet->m_transition[i]->m_event[0];
            if (MleKeyboardPolled::g_keyboardManager->keyDown(event))
			{
				snippetNumber = snippet->m_transition[i]->m_targetIndex;
                snippet = character->m_snippet[snippetNumber];
                frame = snippet->m_startFrame;
                animationPlayMode = TRUE;
			}
		}
}


void Mle3dCharacterActor::behave(void* client)
{
    // Do the actor's behavior
    
    Mle3dCharacterActor* a = (Mle3dCharacterActor*)client;

    a->checkForEvents();

    if (a->animationPlayMode) 
	{
        if ((a->frame < a->snippet->m_startFrame) ||
            (a->frame >= a->snippet->m_endFrame)) 
            a->animationPlayMode = FALSE;
            //a->frame=a->snippet->startFrame;

        if (a->snippet)
		{
            // Update the transform list to reference the list of transforms from
            // the current frame of the animation sequence
            Mle3dSequence* sequence = NULL;
            for (unsigned int i = 0; i < a->sceneGraphSize; i++) 
			{
				if (a->animationObjectNameList && a->animationObjectNameList[i] &&
                    (sequence=Mle3dAnimation::getSequence(a->animation,
                                                          a->animationObjectNameList[i])))
                    a->animationTransformList[i] = &(sequence->m_frame[a->frame]);
                else
                    a->animationTransformList[i] = NULL;
			}
            // Apply the updated transforms to the scene graph
            Mle3dTransformRegistryCarrier::set(a->sceneGraph.getTransformList(),
                                               a->animationTransformList);

            // Increment the frame counter
            // XXX The character animation package will eventually take care of this
            a->frame += a->animationSpeed;
		}

	}
}
