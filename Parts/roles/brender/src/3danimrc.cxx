/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3danimrc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Animation Registry
 * property targeting the BRender platform.
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

// Include system header files.
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/MleLoad.h"
#include "mle/MleRole.h"
#include "mle/3danimrm.h"
#include "mle/3danimrc.h"


Mle3dAnimationRegistry* Mle3dAnimationRegistryCarrier::set(MleRole* role,MediaRef animationMediaRef)
{
  Mle3dAnimationRegistry* animationRegistry = NULL;

  if (animationMediaRef != MLE_NO_MEDIA)
     { Mle3dAnimationRegistryMediaRef* animationMedia =
                   (Mle3dAnimationRegistryMediaRef*)fwLoadMediaRef(animationMediaRef,NULL);
       if (animationMedia)
          { animationRegistry=animationMedia->read();
            if (!animationRegistry)
               fprintf(stderr,
                    "Mle3dAnimationRegistryCarrier::Error--could not read animation registry from media \"%s\".\n",
                     animationMedia->getMediaRefBuffer(animationMedia->getNextMediaRef(NULL)));
            delete animationMedia; 
          }
     }
#ifdef MLE_REHEARSAL
   else
     fprintf(stderr,"%s%s%s",
               "Mle3dAnimationRegistryCarrier::Error--AnimationRegistry MediaRef property for Actor \"",
               role->getActor()->getName(),"\" has no value.\n");
#endif

  return animationRegistry;
}


/*
Mle3dAnimation* Mle3dRegistryAnimationCarrier::setCurrentAnimation(Mle3dAnimation* newAnimation)
{ 
  animation=newAnimation;
  if (animation)
     {  Mle3dRegistryAnimationActor* myActor=(Mle3dRegistryAnimationActor*)getActor();
        myActor->setNumFrames(animation->numFrames);
        if (sequenceList==NULL)
           sequenceList=fw3dGetSequenceList(animation);
        else
          { if (sequenceList)
               { Mle3dSequence* newSequence;
                 for (unsigned int i=0;i<animation->numSequences;i++)
                     { newSequence=fw3dGetSequence(animation,i);
                       if (newSequence->identifier!=sequenceList[i]->identifier)
                          updateActorModel(i,newSequence);
                       sequenceList[i]=newSequence;
                     }
               }
            else sequenceList=fw3dGetSequenceList(animation);
          }
     }
  return animation;
}




Mle3dAnimation* Mle3dRegistryAnimationCarrier::setCurrentAnimation(int animationNumber)
{ Mle3dAnimation* result=NULL;

  if (animationRegistry&&(animationNumber>-1)
      &&(animationNumber<(int)animationRegistry->numAnimations))
     { currentAnimation=animationNumber;
       result=setCurrentAnimation(fw3dGetAnimation(animationRegistry,currentAnimation));
     }
  
  return result;
}


// XXX Right now the animation is assumed to be in the same order as the geometry

br_matrix34** Mle3dRegistryAnimationCarrier::setupSceneGraph()
{
  transformList=NULL;

  if (actorCount&&animation&&(animation->numSequences>0))
     { transformList=new br_matrix34*[animation->numSequences];
       br_actor** geometryList=new br_actor*[actorCount];
       unsigned int i,j,length;

       for (i=0;i<actorCount;i++)
           geometryList[i]=actorList[i];

       unsigned int maxAnimations=animation->numSequences;
       if (maxAnimations>actorCount)
          maxAnimations=actorCount;

       for (i=0;i<maxAnimations;i++)
           { //setup actors for this sequence
             if (sequenceList[i]!=NULL)
                { //Match up the geometry with the animation 
                  //XXX The geometry identifier is a string but the animation identifier is a string
                  char id[50],name[255];
                  sprintf(id,"%d",this);
                  sprintf(name,"%d",sequenceList[i]->identifier);

                  int found=FALSE;
                  if (name) 
                     { length=strlen(name);
                       for (j=0;j<actorCount;j++)
                           if ((geometryList[j]->identifier)&&
                               (strncmp(name,geometryList[j]->identifier,length)==0))
                              {  actorList[i]=geometryList[j];
                                 j=actorCount;
                                 found=TRUE;
                              }
                     }
                  
                  if (found)
                     { //Load transforms for first frame
                       transformList[i]=&(actorList[i]->t.t.mat);
                       #ifdef MLE_REHEARSAL
                          memcpy(transformList[i]->m,
                                 (sequenceList[i]->frame[0]).matrix,sizeof(br_matrix34));
                       #else //not MLE_REHEARSAL
                          for (int m=0;m<4;m++)
                              for (int n=0;n<3;n++)
                                  transformList[i]->m[m][n]=
                                    ///XXX This won't work if fixed point brender is used
                                    (br_scalar)(sequenceList[i]->frame[0].matrix[m][n]);
                       #endif
                     }
                }
           }
     }
     
  return transformList;
}


int Mle3dRegistryAnimationCarrier::updateActorModel(long target,Mle3dSequence* sequence)
{ int result=FALSE;

  char name[255];
  sprintf(name,"%d",sequence->identifier);
  for (unsigned int i=0;i<getGeometryRegistrySize();i++)
       if (strncmp(name,geometryRegistry[i]->identifier,getGeometryRegistrySize())==0)
          { actorList[target]->model=geometryRegistry[i];
            i=getGeometryRegistrySize();
            result=TRUE;
          }

  return result;
}


void Mle3dRegistryAnimationCarrier::update(void* client)
{
  Mle3dRegistryAnimationCarrier* d = (Mle3dRegistryAnimationCarrier*)client;
  Mle3dRegistryAnimationActor* a = (Mle3dRegistryAnimationActor*)d->getActor();
  
  if ((d!=NULL)&&(d->animation!=NULL)&&(d->animation->numSequences))
     { d->currentFrame=a->currentFrame;
       if (a->currentAnimation!=d->currentAnimation)
          { d->setCurrentAnimation(a->currentAnimation);
            d->currentFrame=a->currentFrame=0;
            d->previousFrame=-1;
          }
       if (d->currentFrame!=d->previousFrame)
          { //Here's where we copy the current frame's matrices into the scene graph nodes
            #ifndef MLE_REHEARSAL
            int m,n;
            #endif
            for (unsigned int i=0;i<d->animation->numSequences;i++)
                #ifdef MLE_REHEARSAL
                //We can do a fast memcpy because both types are MleTransforms
                   memcpy(d->transformList[i]->m,
                          (d->sequenceList[i]->frame[d->currentFrame]).matrix,sizeof(br_matrix34));
                #else //not MLE_REHEARSAL
                   for (m=0;m<4;m++)
                       for (n=0;n<3;n++)
                           d->transformList[i]->m[m][n]=
                              ///XXX This won't work if fixed point brender is used
                              (br_scalar)(d->sequenceList[i]->frame[d->currentFrame].matrix[m][n]);
                #endif
                d->previousFrame=d->currentFrame;
          }
     }
}

*/
