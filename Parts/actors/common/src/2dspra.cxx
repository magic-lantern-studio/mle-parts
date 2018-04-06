/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dspra.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 2D Sprite Actor.
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

#include "mle/2dspra.h"
#include "mle/2dsprd.h"


#define GET_SPRITE_ROLE(myrole) \
  Mle2dSpriteRole * myrole = (Mle2dSpriteRole *) getRole(); \
  MLE_ASSERT(myrole);  

#define STRING_EQUALP(a,b) (strcmp(a,b) == 0)

MLE_ACTOR_SOURCE(Mle2dSpriteActor, MleActor);


#if defined(MLE_REHEARSAL)
void
Mle2dSpriteActor::initClass(void)
{
    mlRegisterActorClass(Mle2dSpriteActor, MleActor);

    mlRegisterActorMember(Mle2dSpriteActor, spriteData, MlMediaRef);
    mlRegisterActorMember(Mle2dSpriteActor, position, MlVector2);
    mlRegisterActorMember(Mle2dSpriteActor, currentPage, int);
    mlRegisterActorMember(Mle2dSpriteActor, pageCount, int);
    mlRegisterActorMember(Mle2dSpriteActor, pageWidth, int);
    mlRegisterActorMember(Mle2dSpriteActor, pageHeight, int);
    mlRegisterActorMember(Mle2dSpriteActor, visible, int);
    mlRegisterActorMember(Mle2dSpriteActor, xscale, MlScalar);
    mlRegisterActorMember(Mle2dSpriteActor, yscale, MlScalar);
    mlRegisterActorMember(Mle2dSpriteActor, frameInterval, int);

    //
    // Mark all the properties that belongs to the "transform" property
    // data set.
    //
    mlRegisterActorMemberDataset(Mle2dSpriteActor, position, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle2dSpriteActor, xscale, MLE_PROP_DATASET_TRANSFORM);
    mlRegisterActorMemberDataset(Mle2dSpriteActor, yscale, MLE_PROP_DATASET_TRANSFORM);
}

void
Mle2dSpriteActor::resolveEdit(const char *property)
{
  GET_SPRITE_ROLE(role);

  if (property) {
    if (STRING_EQUALP(property, "position"))
      role->setPosition(position);
    else if (STRING_EQUALP(property, "visible"))
      role->setVisible(visible);
    else if (STRING_EQUALP(property, "currentPage"))
      role->setCurrentPage(currentPage);
    else if (STRING_EQUALP(property, "xscale") ||
	     STRING_EQUALP(property, "yscale"))
      role->setScale(xscale, yscale);
    else if (STRING_EQUALP(property, "frameInterval")) {
      // Remove currently scheduled update function
      g_theTitle->m_scheduler->remove(this);
      // Re-schedule sprite update function using new frame rate.
      checkFrameInterval();
      g_theTitle->m_scheduler->insertFunc(PHASE_ACTOR,(MleSchedFunc) Mle2dSpriteActor::update,
				      this,this,
				      (unsigned int) frameInterval,
				      1);
    }
  }
}
#endif // MLE_REHEARSAL

Mle2dSpriteActor::Mle2dSpriteActor(void)
 : MleActor()
{
  // No media has been loaded
  spriteData = MLE_NO_MEDIA;

  // Start with the first sprite page.
  currentPage = pageCount = pageWidth = pageHeight = 0; 

  // Set position to lower-left corner.
  position.setValue(ML_SCALAR(0), ML_SCALAR(0));

  // Set scale to unity (i.e., to the actual size
  // of the media image).
  xscale = yscale = ML_SCALAR(1.0);

  // Make the sprite initially invisible
  visible = FALSE;
  
  // Initialize "frame rate" to the fastest it can be.
  frameInterval = 1;
}


Mle2dSpriteActor::~Mle2dSpriteActor(void)
{
  // Role releases all media resources
}


void
Mle2dSpriteActor::init(void)
{
  GET_SPRITE_ROLE(role);

  // Push state to role
  push();

  // Load sprite media
  MlBoolean spriteMediaLoaded = load(spriteData);
  MLE_ASSERT(spriteMediaLoaded == TRUE);
  
  // Schedule sprite update function.
  checkFrameInterval();
  theTitle->scheduler->insertFunc(PHASE_ACTOR,(MleSchedFunc) Mle2dSpriteActor::update,
				  this,this,
				  (unsigned int) frameInterval,
				  1);
}


void
Mle2dSpriteActor::setPosition(MlVector2 &position)
{
  GET_SPRITE_ROLE(role);

  // Set actor's position.
  position.setValue(position.getValue());

  // Send position to role.
  role->setPosition(position);
}

int
Mle2dSpriteActor::getPageCount(void)
{
  return pageCount;
}

int
Mle2dSpriteActor::getPageWidth()
{
  return pageWidth;
}

int
Mle2dSpriteActor::getPageHeight()
{
  return pageHeight;
}

MlScalar
Mle2dSpriteActor::getXScale(void)
{
  return xscale;
}

MlScalar
Mle2dSpriteActor::getYScale(void)
{
  return yscale;
}

#if defined(MLE_REHEARSAL) || defined(__sgi)
void
Mle2dSpriteActor::setScale(MlScalar deltaw, MlScalar deltah)
{
  GET_SPRITE_ROLE(role);

  xscale = deltaw;
  yscale = deltah;

  role->setScale(xscale, yscale);
}
#endif // MLE_REHEARSAL or __sgi

int
Mle2dSpriteActor::getCurrentPage()
{
  return currentPage;
}

void
Mle2dSpriteActor::setCurrentPage(int number)
{
  GET_SPRITE_ROLE(role);

  currentPage = number;

  role->setCurrentPage(currentPage);
}

MlBoolean
Mle2dSpriteActor::load(MlMediaRef spriteData)
{
  GET_SPRITE_ROLE(role);
  MlBoolean spriteMediaLoaded;

  spriteMediaLoaded = role->load(spriteData);
  
  return spriteMediaLoaded;
}

int
Mle2dSpriteActor::getVisible(void)
{
  return visible;
}

void
Mle2dSpriteActor::setVisible(int yesOrNo)
{
  GET_SPRITE_ROLE(role);

  visible = yesOrNo;

  role->setVisible(visible);
}

void
Mle2dSpriteActor::push(void)
{
  GET_SPRITE_ROLE(role);

  role->setPageCount(pageCount);
  role->setPageHeight(pageHeight);
  role->setPageWidth(pageWidth);
  role->setPosition(position);
  role->setVisible(visible);
  role->setCurrentPage(currentPage);
  role->setScale(xscale, yscale);
}

void
Mle2dSpriteActor::update(Mle2dSpriteActor *sprite)
{
  Mle2dSpriteRole * role = (Mle2dSpriteRole *) sprite->getRole();
  MLE_ASSERT(role);  

  sprite->currentPage += 1;
  if (sprite->currentPage >= sprite->pageCount)
    sprite->currentPage = 0;
  role->setCurrentPage(sprite->currentPage);
}

// checkFrameInterval() is a private function
// to ensure that the frameInterval limits
// are not violated.
void
Mle2dSpriteActor::checkFrameInterval(void)
{
  // Ensure that frameInterval does not exceed limits
  if (frameInterval < 0)
    frameInterval = 0;  
}

