/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file rtestubs.cpp
 * @ingroup MleParts
 *
 * This file implements stubs for the Qt Set unit test.
 *
 * @author Mark S. Millard
 * @date October 2, 2017
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017 Wizzer Works
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// This file contains stubs for Magic Lantern Runtime Engine structures
// that are genererated using the DigitalPlayprint mastering tools.
// This test does not require mastered code, and therefore can use
// stubbed declarations.

// Include system header files.
#include <stdlib.h>

// Include Magic Lantern Runtime Engine header files.
# include "mle/MleTables.h"

// Runtime Engine support for Actors.
MleRTPropertyEntry mlRTActorProperties[1];
MleRTActorClassEntry mlRTActorClass[1];
int mlRTActorPropertyLength = 0;
int mlRTActorClassLength = 0;

// Runtime Engine support for Roles.
MleRTRoleClassEntry mlRTRoleClass[1];
int mlRTRoleClassLength = 0;

// Runtime Engine support for Media References.
MleRTMediaRefEntry mlRTMediaRef[1];
int mlRTMediaRefLength = 0;

// Runtime Engine support for Sets.
MleRTPropertyEntry mlRTSetProperties[1];
MleRTSetClassEntry mlRTSetClass[1];
MleRTSetEntry mlRTSet[1];
int mlRTSetPropertyLength = 0;
int mlRTSetClassLength = 0;
int mlRTSetLength = 0;
int MleRTSetChunk[1];

// Runtime Engine support for Scenes.
MleRTSceneClassEntry mlRTSceneClass[1];
int mlRTSceneClassLength = 0;
int MleDppBootScene = 0;

// Runtime Engine support for Groups.
MleRTGroupClassEntry mlRTGroupClass[1];
int mlRTGroupClassLength = 0;
