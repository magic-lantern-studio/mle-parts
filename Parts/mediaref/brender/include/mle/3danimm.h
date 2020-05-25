/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3danimm.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Animation Media Reference
 * targeting the BRender platform.
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

#ifndef __3DANIMM_H_
#define __3DANIMM_H_

#ifdef psx
#define FILE int
#endif

// Include system header files.
#include <stdio.h>

// Include Runtime Player header files.
#include "mle/MleMediaRef.h"


#ifndef  MLE_UNKNOWN
#define  MLE_UNKNOWN 1
#endif // MLE_UNKNOWN

class Mle3dAnimationRegistry;

class Mle3dAnimationMedia : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dAnimationMedia);

  public:

    Mle3dAnimationMedia();
    virtual                  ~Mle3dAnimationMedia();
    Mle3dAnimationRegistry*   read();
    Mle3dAnimationRegistry*   getAnimationRegistry() { return m_animationRegistry; }
    static Mle3dAnimationRegistry*   loadAnimationRegistry(char* filename);

  private:

    Mle3dAnimationRegistry*  m_animationRegistry;
};


#define MLE_MAGIC_3D_ANIMATION_REGISTRY "FWa1"


class Mle3dAnimationTransform
{
  public:

  // XXX - This won't handle fixed point this way.
  float m_matrix[4][3];
};
void ml3dPrintTransform(Mle3dAnimationTransform* t);
void ml3dPrintTransform(Mle3dAnimationTransform* t, FILE* out);


class Mle3dSequence
{
  public:

    unsigned long          m_identifier;
    unsigned long          m_parent;
    unsigned short         m_startFrame;
    unsigned short         m_type;
    Mle3dAnimationTransform m_frame[MLE_UNKNOWN];
};
void ml3dPrintSequence(Mle3dSequence* sequence);
void ml3dPrintSequence(Mle3dSequence* sequence, FILE *out);


class Mle3dAnimation
{
  public:

    unsigned long    m_identifier;
    unsigned long    m_numSequences;
    unsigned long    m_numFrames;
    // An array of Mle3dSequences follows next, indexed by computing sizes.
};

void            ml3dPrintAnimation(Mle3dAnimation* animation);
void            ml3dPrintAnimation(Mle3dAnimation* animation, FILE* out);
Mle3dSequence*  ml3dGetSequence(Mle3dAnimation* animation, unsigned int index);
Mle3dSequence** ml3dGetSequenceList(Mle3dAnimation* animation);


class Mle3dAnimationRegistry
{
  public:

    unsigned long    m_magicNumber;
    unsigned long    m_numAnimations;
    unsigned long    m_animationOffset[MLE_UNKNOWN];
    // An array of Mle3dAnimations follows next, indexed by the animationOffsets.
};
Mle3dAnimation* ml3dGetAnimation(Mle3dAnimationRegistry* registry, unsigned int index);
void            ml3dPrintAnimationRegistry(Mle3dAnimationRegistry* registry);
void            ml3dPrintAnimationRegistry(Mle3dAnimationRegistry* registry, FILE *out);



#endif /* __3DANIMM_H_ */
