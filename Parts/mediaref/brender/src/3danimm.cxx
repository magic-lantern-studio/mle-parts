/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3danimm.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Animation Media Reference
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

// Include system header files.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>

#ifdef WIN32
#include <io.h>
#else // not WIN32
#include <unistd.h>
#endif 

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlFileIO.h"
#include "mle/brendapi.h"
#include "mle/3danimm.h"


MLE_MEDIAREF_SOURCE(Mle3dAnimationMedia,MleMediaRef);


Mle3dAnimationMedia::Mle3dAnimationMedia()
{
    m_animationRegistry = NULL;
}


Mle3dAnimationMedia::~Mle3dAnimationMedia()
{
    // Do nothing.
}


Mle3dAnimationRegistry* Mle3dAnimationMedia::read()
{
    // Declare the return structure.
	char *buffer = m_references->buffer;
    Mle3dAnimationRegistry* animationRegistry = NULL;

    // Read BRender Model Registry from external reference.
    if (buffer) 
        animationRegistry = loadAnimationRegistry((char*)buffer);

    if (! animationRegistry)
        fprintf(stderr,"Mle3dAnimationMedia::Warning--No animation found in animation registry.\n");


    return animationRegistry;
}


Mle3dAnimationRegistry* Mle3dAnimationMedia::loadAnimationRegistry(char* filename)
{
	Mle3dAnimationRegistry* myRegistry = NULL;
  
    if (filename)
    {
        unsigned int filesize = 0;
        int statcheck = mlOpen(filename,O_RDONLY);
        if (statcheck>-1)
        {
			struct stat status;
            fstat(statcheck,&status);
            filesize = status.st_size;
            mlClose(statcheck);

            if (filesize)
            {
				FILE* in = mlFOpen(filename,"rb");
                if (in != NULL)
                {
					myRegistry = (Mle3dAnimationRegistry*)mlMalloc(filesize);
                    if (mlFRead(myRegistry,filesize,1,in) == 0)
                        myRegistry = NULL;
                    mlFClose(in);

                    if (myRegistry)
                    { 
						// First-pass check to make sure file has magic number for animation registry.
                        char* magic = MLE_MAGIC_3D_ANIMATION_REGISTRY;
                        unsigned long* check=(unsigned long*)magic;
                        if (*check != myRegistry->m_magicNumber)
                        {
							fprintf(stderr,"%s%s%s","Mle3dAnimationMedia::Error--file ",filename,
                                     " is not a Magic Lantern animation registry.\n");
                            delete [] myRegistry;
                            myRegistry=NULL;
                        }
                    }

                    if (myRegistry)
                    {
						// Second-pass check to see if animation information in file is possibly invalid.
                        unsigned int check = 0;
                        for (unsigned int i = 0; i < myRegistry->m_numAnimations; i++)
                            if (myRegistry->m_animationOffset[i] > check)
                                check = myRegistry->m_animationOffset[i];
                        if (check > filesize)
                        {
						    delete [] myRegistry;
                            myRegistry = NULL;
                        }
                    }
                }
            }  
       
            if (! myRegistry)
               fprintf(stderr,"Mle3dAnimationMedia::Error--Could not load animation from file %s.\n",filename);
        }
        else
            fprintf(stderr,"Mle3dAnimationMedia::Error--Could not find animation registry file %s.\n",filename);
    }
    else
        fprintf(stderr,"Mle3dAnimationMedia::Error--No filename given for animation registry.\n");

    return myRegistry;
}


void ml3dPrintTransform(Mle3dAnimationTransform* t, FILE* out)
{
    if (t&&out)
    {
		fprintf(out,"\t\t%10.4f %10.4f %10.4f %10.4f\n",
                t->m_matrix[0][0], t->m_matrix[0][1],
                t->m_matrix[0][2], 0.0);
        fprintf(out,"\t\t%10.4f %10.4f %10.4f %10.4f\n",
                t->m_matrix[1][0], t->m_matrix[1][1],
                t->m_matrix[1][2], 0.0);
        fprintf(out,"\t\t%10.4f %10.4f %10.4f %10.4f\n",
                t->m_matrix[2][0], t->m_matrix[2][1],
                t->m_matrix[2][2], 0.0);
        fprintf(out,"\t\t%10.4f %10.4f %10.4f %10.4f\n",
                t->m_matrix[3][0], t->m_matrix[3][1],
                t->m_matrix[3][2], 0.0);
	}
}


void ml3dPrintTransform(Mle3dAnimationTransform* t)
{
    ml3dPrintTransform(t,stdout);
}


Mle3dAnimation* ml3dGetAnimation(Mle3dAnimationRegistry* registry, unsigned int index)
{
    return((Mle3dAnimation*)(((char*)registry)+registry->m_animationOffset[index]));
};


Mle3dSequence* ml3dGetSequence(Mle3dAnimation* animation, unsigned int index)
{
    return((Mle3dSequence*)(((char*)animation) + 3 * sizeof(animation->m_identifier)
        + index*(sizeof(Mle3dSequence)
        + (animation->m_numFrames - MLE_UNKNOWN) * sizeof(Mle3dAnimationTransform))));
}


Mle3dSequence** ml3dGetSequenceList(Mle3dAnimation* animation)
{
    Mle3dSequence** sequenceList = new Mle3dSequence*[animation->m_numSequences];
    for (unsigned int i = 0; i < animation->m_numSequences; i++)
        sequenceList[i] = ml3dGetSequence(animation,i);

    return sequenceList;
}


void ml3dPrintAnimation(Mle3dAnimation* animation, FILE* out)
{ 
    if (animation&&out)
    {
        fprintf(out,"Number of Sequences: %d\n",animation->m_numSequences);
        fprintf(out,"Number of Frames: %d\n",animation->m_numFrames);
     
        Mle3dSequence* sequence;
        for (unsigned int i=0;i<animation->m_numSequences;i++)
        { 
             fprintf(out,"Sequence %d: ",i);
             sequence = ml3dGetSequence(animation,i);
             fprintf(out,"identifier=%d\n",sequence->m_identifier);
             fprintf(out,"            parent=%d\n",sequence->m_parent);
             fprintf(out,"            startFrame=%d\n",sequence->m_startFrame);
             fprintf(out,"            type=%d\n",sequence->m_type);
             fprintf(out,"            First Transform:\n");
             ml3dPrintTransform(&(sequence->m_frame[0]),out);
        }
    }
}


void ml3dPrintAnimation(Mle3dAnimation* animation)
{
    ml3dPrintAnimation(animation,stdout);
}


void ml3dPrintAnimationRegistry(Mle3dAnimationRegistry* registry, FILE* out)
{
    if (registry&&out)
    {
		fprintf(out,"Number of Animations=%d\n",registry->m_numAnimations);
        for (unsigned int i = 0; i < registry->m_numAnimations; i++)
            ml3dPrintAnimation(ml3dGetAnimation(registry,i),out);
    }
}


void ml3dPrintAnimationRegistry(Mle3dAnimationRegistry* registry)
{
    ml3dPrintAnimationRegistry(registry,stdout);
}
