/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file player.h
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2025 Wizzer Works
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

#ifndef __ML_PLAYER_H_
#define __ML_PLAYER_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/MleRuntime.h"
#include "mle/DwpItem.h"

// Define a function pointer type.
typedef MleDwpItem * (*PlayerSendGetDwpItem)(const char *);

typedef struct _playerStruct {
	PlayerSendGetDwpItem sendGetWorkprintGroup;
	PlayerSendGetDwpItem sendGetWorkprintScene;
	PlayerSendGetDwpItem sendGetWorkprintMediaRef;
	PlayerSendGetDwpItem sendGetWorkprintSet;
} MlePlayerStruct;

extern MLE_RUNTIME_API MlBoolean mlInitPlayer();

extern MLE_RUNTIME_API MlBoolean mlClosePlayer();

extern MLE_RUNTIME_API MlePlayerStruct* mlGetPlayer();

extern MLE_RUNTIME_API void mlAddGetWorkprintGroup(PlayerSendGetDwpItem);

extern MLE_RUNTIME_API void mlClearGetWorkprintGroup();

extern MLE_RUNTIME_API MleDwpItem * mlGetWorkprintGroup(const char*);

extern MLE_RUNTIME_API void mlAddGetWorkprintScene(PlayerSendGetDwpItem);

extern MLE_RUNTIME_API void mlClearGetWorkprintScene();

extern MLE_RUNTIME_API MleDwpItem * mlGetWorkprintScene(const char*);

extern MLE_RUNTIME_API void mlAddGetWorkprintSet(PlayerSendGetDwpItem);

extern MLE_RUNTIME_API void mlClearGetWorkprintSet();

extern MLE_RUNTIME_API MleDwpItem * mlGetWorkprintSet(const char*);

extern MLE_RUNTIME_API void mlAddGetWorkprintMediaRef(PlayerSendGetDwpItem);

extern MLE_RUNTIME_API void mlClearGetWorkprintMediaRef();

extern MLE_RUNTIME_API MleDwpItem * mlGetWorkprintMediaRef(const char*);

#endif /* __ML_PLAYER_H_ */
