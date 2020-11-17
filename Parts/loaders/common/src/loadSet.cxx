/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file loadSet.cxx
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2003-2020 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this source code, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include System header files.
#include <stdio.h>

// Include Magic Lantern header files.
#include <mle/mlAssert.h>
#include <mle/mlFileio.h>

// Include Magic Lantern Player header files.
#include "mle/MleLoad.h"
#include "mle/MleSet.h"
#include "mle/MleDirector.h"

#ifdef MLE_DIGITAL_WORKPRINT

#include "mle/MleDso.h"
#include "mle/MleSetClass.h"

// Include Magic Lantern Digital Workprint header files.
#include "mle/DwpSet.h"
#include "mle/DwpProperty.h"

/*
 * This static function is a convenience function to create a
 * Set from a MleDwpSet, the Set workprint item.  It does
 * the work of referring to the Set registry, creating the
 * Set, finding property values and poking them in.
 */
static MleSet *
_mlCreateSet(MleDwpSet *wpi)
{
    /* Look up Set class. */
    const MleSetClass *sc = MleSetClass::find(wpi->getType());

    if ( sc == NULL )
    {
        /* Try loading the DSO. */
        if ( mlLoadDSO(wpi->getType()) )
        {
            printf("***** ERROR loading DSO for %s.\n",wpi->getType());
            return NULL;
        }

        /* Try to look up the class again. */
        sc = MleSetClass::find(wpi->getType());
    }

    if ( sc == NULL )
        return NULL;

    /* Create the Set. */
    MleSet *set = sc->createInstance();

    /* Register with instance registry. */
    if (set) set->registerInstance(wpi->getName());
    
    /* Apply properties. */
    MleDwpItem *prop = wpi->getFirstChild();
    while ( prop )
    {
#if defined(WIN32)
        if ( prop->isa("MleDwpProperty") )
#else
		if ( prop->isa(MleDwpProperty::typeId) )
#endif
            set->poke(((MleDwpProperty *)prop)->getName(),
                &((MleDwpProperty *)prop)->m_data);
        
        prop = prop->getNext();
    }

    return set;
}

// Rehearsal time version of mlLoadSet (note it takes a string set name)
//   This function relies on extern functions to deliver a workprint
//   specification of the Set to load:
//
//	MleDwpSet *_mlGetWorkprintSet(const char *setname);
//	void _mlReleaseWorkprintSet(MleDwpItem *item);
//
//   The first should return a pointer to a workprint Set, which may
//   contain other configuring items.  The second is a call to release
//   the group, in case this is a temporary allocation.
//
MleSet *
mlLoadSet(const char *id)
{
	// Declare functions.
	extern MleDwpSet *_mlGetWorkprintSet(const char *setname);
	extern void _mlReleaseWorkprintSet(MleDwpItem *item);

	// Get the workprint item.
	MleDwpSet *wpset = _mlGetWorkprintSet(id);
	if ( wpset == NULL )
	{
		printf("can't find set %s in workprint.\n",id);
		return NULL;
	}

	// Create the Set.
	MleSet *set = _mlCreateSet(wpset);
	if(! set)
	{
		printf("error creating %s, a set of type %s.\n",
			wpset->getName(), wpset->getType());
		// release the workprint
        _mlReleaseWorkprintSet(wpset);
        return NULL;
	}

	// Run the initialization function.
	set->init();

	// Put the set in the registry.
	set->registerInstance(wpset->getName());

	// Release the workprint.
	_mlReleaseWorkprintSet(wpset);

	// and return the set.
	return set;
}

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleActorGC.h"
#include "mle/MleTables.h"

// Include Digital Playprint header files.
#include "mle/ppinput.h"


static void
_mlCreateSet(int id)
{
    int chunkTOCIndex;
    long curFileOffset, setChunkOffset;
    unsigned char *setChunk, *tmpSetChunk;
    unsigned int setChunkLength;
    FILE *ppFilePtr;

    /*
     * Make sure there is a playprint file pointer.
     */
    MLE_ASSERT(g_theTitle->m_dpp);
    ppFilePtr = g_theTitle->m_dpp->getFp();
    MLE_ASSERT(ppFilePtr);

    /*
     * Save the current position of the playprint.
     */
    curFileOffset = mlFTell(ppFilePtr);

    /*
     * Seek to the beginning of the appropriate Set chunk.
     * XXX Mark will do the clean up to make use of the tag and size of
     *     MleDppChunk structure correctly.
     */
    chunkTOCIndex = MleRTSetChunk[id];
    setChunkOffset = g_theTitle->m_dpp->getTOCOffset((unsigned long) chunkTOCIndex);
    mlFSeek(ppFilePtr, setChunkOffset + sizeof(MlDppTag), SEEK_SET);

    /*
     * Allocate memory to store the Set chunk, read in the size and content
     * of the Set chunk.
     */
    mlFRead(&setChunkLength, sizeof(unsigned int), 1, ppFilePtr);
    setChunk = new unsigned char[setChunkLength];
    mlFRead(setChunk, setChunkLength, 1, ppFilePtr);

    /*
     * Interpret the Set chunk, create the Set, initialize its property
     * values if any.
     */
    tmpSetChunk = setChunk;
    unsigned char *&setChunkRef = tmpSetChunk;
    parseStream(setChunkRef, setChunk, setChunk + setChunkLength,
		MLE_CHUNK_SET);

    delete [] setChunk;

    /*
     * Restore the current position of the playprint.
     */
    mlFSeek(ppFilePtr, curFileOffset, SEEK_SET);
}


// This is the run-time version of the Set loader/creator.
MleSet *
mlLoadSet(const int id)
{
    MLE_ASSERT(id >= 0);
    MLE_ASSERT(id < mlRTSetLength);

    if (! mlRTSet[id].ptr)
	{
		// Find the Set chunk in the playprint, create the Set,
		// and initialize the Set property values.
		_mlCreateSet(id);

		// Run the initialization function.
		(mlRTSet[id].ptr)->init();
    }

    return mlRTSet[id].ptr;
}

#endif /* MLE_DIGITAL_PLAYPRINT */
