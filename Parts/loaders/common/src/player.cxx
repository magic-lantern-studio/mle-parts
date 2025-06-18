/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file player.cxx
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/player.h"


// Internal lookup table for Player tools
static MlePlayerStruct *_player = nullptr;


MlBoolean mlInitPlayer()
{
	if (_player == nullptr) {
		_player = (MlePlayerStruct *)mlMalloc(sizeof(MlePlayerStruct));
		if (! _player) return ML_FALSE;
	}
	else {
		return ML_TRUE;
	}

	_player->sendGetWorkprintGroup = nullptr;
	_player->sendGetWorkprintScene = nullptr;
	_player->sendGetWorkprintMediaRef = nullptr;
	_player->sendGetWorkprintSet = nullptr;

	return ML_TRUE;
}


MlBoolean mlClosePlayer()
{
	if (_player != nullptr) {
		mlFree(_player);
	}
	return ML_TRUE;
}


MlePlayerStruct* mlGetPlayer()
{
	return _player;
}


void mlAddGetWorkprintGroup(PlayerSendGetDwpItem cb)
{
	if (_player != nullptr)
	    _player->sendGetWorkprintGroup = cb;
}


void mlClearGetWorkprintGroup()
{
	if (_player != nullptr)
	    _player->sendGetWorkprintGroup = nullptr;
}


MleDwpItem *mlGetWorkprintGroup(const char *id)
{
	if (_player == nullptr)
		return nullptr;

	if (_player->sendGetWorkprintGroup != nullptr)
		return _player->sendGetWorkprintGroup(id);
	else
		return nullptr;
}


void mlAddGetWorkprintScene(PlayerSendGetDwpItem cb)
{
	if (_player != nullptr)
    	_player->sendGetWorkprintScene = cb;
}


void mlClearGetWorkprintScene()
{
	if (_player != nullptr)
	    _player->sendGetWorkprintScene = nullptr;
}


MleDwpItem * mlGetWorkprintScene(const char* id)
{
	if (_player == nullptr)
		return nullptr;

	if (_player->sendGetWorkprintScene != nullptr)
		return _player->sendGetWorkprintScene(id);
	else
		return nullptr;
}


void mlAddGetWorkprintMediaRef(PlayerSendGetDwpItem cb)
{
	if (_player != nullptr)
    	_player->sendGetWorkprintMediaRef = cb;
}


void mlClearGetWorkprintMediaRef()
{
	if (_player != nullptr)
	    _player->sendGetWorkprintMediaRef = nullptr;
}


MleDwpItem * mlGetWorkprintMediaRef(const char* id)
{
	if (_player == nullptr)
		return nullptr;

	if (_player->sendGetWorkprintMediaRef != nullptr)
		return _player->sendGetWorkprintMediaRef(id);
	else
		return nullptr;
}


void mlAddGetWorkprintSet(PlayerSendGetDwpItem cb)
{
	if (_player != nullptr)
	    _player->sendGetWorkprintSet = cb;
}


void mlClearGetWorkprintSet()
{
	if (_player != nullptr)
	    _player->sendGetWorkprintSet = nullptr;
}


MleDwpItem * mlGetWorkprintSet(const char* id)
{
	if (_player == nullptr)
		return nullptr;

	if (_player->sendGetWorkprintSet != nullptr)
		return _player->sendGetWorkprintSet(id);
	else
		return nullptr;
}
