/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file loadDwp.cxx
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2025 Wizzer Works
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

// Include system header files.
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlAssert.h"

// Include Digital Workprint header files.
#include "mle/DwpItem.h"
#include "mle/DwpDSOFile.h"
#include "mle/DwpSet.h"
#include "mle/DwpGroup.h"
#include "mle/DwpScene.h"
#include "mle/DwpMediaRef.h"
#include "mle/DwpFinder.h"
#include "mle/DwpLoad.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleRuntime.h"

#ifdef MLE_REHEARSAL
#if 0
// Include ATK header files.
#include "mle/MlePlayer.h"

MLE_RUNTIME_API MlePlayer *_mlPlayer = NULL;
#else
#include "mle/player.h"
#endif
#endif /* MLE_REHEARSAL */

static void releaseItem(MleDwpItem *item)
{
#ifdef MLE_REHEARSAL
#if 0
	if (_mlPlayer)
#else
	if (mlGetPlayer() != nullptr)
#endif /* 0 */
	{
		// Find the root.
		while ( item->getParent() )
			item = item->getParent();
		
		delete item;
	}
#endif /* MLE_REHEARSAL */
}

MleDwpGroup *_mlGetWorkprintGroup(const char *id)
{
#ifdef MLE_REHEARSAL
#if 0
	if ( _mlPlayer )
		mlSetWorkprint(_mlPlayer->sendGetWorkprintGroup(id));
#else
	mlSetWorkprint(mlGetWorkprintGroup(id));
#endif /* 0 */
#endif /* MLE_REHEARSAL */

	MLE_ASSERT(mlGetWorkprint());
#if defined(WIN32)
    MleDwpFinder groupFinder("MleDwpGroup",id,0);
#else
	MleDwpFinder groupFinder(MleDwpGroup::typeId,id,0);
#endif
	return (MleDwpGroup *)groupFinder.find(mlGetWorkprint());
}

void _mlReleaseWorkprintGroup(MleDwpItem *group)
{
	releaseItem(group);
}

MleDwpScene *_mlGetWorkprintScene(const char *id)
{
#ifdef MLE_REHEARSAL
#if 0
	if ( _mlPlayer )
		mlSetWorkprint(_mlPlayer->sendGetWorkprintScene(id));
#else
	mlSetWorkprint(mlGetWorkprintScene(id));
#endif /* 0 */
#endif /* MLE_REHEARSAL */

	MLE_ASSERT(mlGetWorkprint());
#if defined(WIN32)
	MleDwpFinder sceneFinder("MleDwpScene",id,0);
#else
	MleDwpFinder sceneFinder(MleDwpScene::typeId,id,0);
#endif
	return (MleDwpScene *)sceneFinder.find(mlGetWorkprint());
}

void _mlReleaseWorkprintScene(MleDwpItem *scene)
{
	releaseItem(scene);
}

MleDwpMediaRef *_mlGetWorkprintMediaRef(const char *id)
{
#ifdef MLE_REHEARSAL
#if 0
	if ( _mlPlayer )
	{
		mlSetWorkprint(_mlPlayer->sendGetWorkprintMediaRef(id));

		// If we fail, just return NULL.
		if ( mlGetWorkprint() == NULL )
			return NULL;
	}
#else
    mlSetWorkprint(mlGetWorkprintMediaRef(id));

	// If we fail, just return NULL.
	if (mlGetWorkprint() == NULL)
		return NULL;
#endif /* 0 */
#endif /* MLE_REHEARSAL */

	MLE_ASSERT(mlGetWorkprint());
#if defined(WIN32)
	MleDwpFinder mediaRefFinder("MleDwpMediaRef",id,0);
#else
	MleDwpFinder mediaRefFinder(MleDwpMediaRef::typeId,id,0);
#endif
	return (MleDwpMediaRef *)mediaRefFinder.find(mlGetWorkprint());
}

void _mlReleaseWorkprintMediaRef(MleDwpMediaRef *mediaRef)
{
	releaseItem(mediaRef);
}

MleDwpSet *_mlGetWorkprintSet(const char *id)
{
#ifdef MLE_REHEARSAL
#if 0
	if ( _mlPlayer )
	{
		mlSetWorkprint(_mlPlayer->sendGetWorkprintSet(id));

		// If we fail, just return NULL.
		if ( mlGetWorkprint() == NULL )
			return NULL;
	}
#else
	mlSetWorkprint(mlGetWorkprintSet(id));

	// If we fail, just return NULL.
	if (mlGetWorkprint() == NULL)
		return NULL;
#endif /* 0 */
#endif /* MLE_REHEARSAL */

	MLE_ASSERT(mlGetWorkprint());
#if defined(WIN32)
	MleDwpFinder setFinder("MleDwpSet",id,0);
#else
	MleDwpFinder setFinder(MleDwpSet::typeId,id,0);
#endif
	return (MleDwpSet *)setFinder.find(mlGetWorkprint());
}

void _mlReleaseWorkprintSet(MleDwpItem *set)
{
	releaseItem(set);
}
