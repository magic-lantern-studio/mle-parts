/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file stabiliz.cxx
 * @ingroup MleParts
 *
 * This file defines a class for a Stabilizer.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2024 Wizzer Works
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
//  For information concerning this source file, contact Mark S. Millard,
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
#include "mle/stabiliz.h"
#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"


Stabilizer::Stabilizer(SimpleMass& simplemass,MlBoolean schedFlag)
 : sm(simplemass)
{
	// insert the update function in the scheduler if requested
	if ( schedFlag )
		g_theTitle->m_theScheduler->insertFunc(
			PHASE_ACTOR,(MleSchedulerFunc)update,this,this);
}

Stabilizer::~Stabilizer()
{
	g_theTitle->m_theScheduler->remove(this);
}

#define COSTHETA ML_SCALAR(0.95f)

void
Stabilizer::update(Stabilizer *s)
{
	// find out what the new velocity would be
	MlVector3 velocity = s->sm.velocity + s->sm.acceleration;

	// localize this velocity
	MlVector3 localVelocity;
	s->sm.localizeVector(velocity,localVelocity);

	if ( localVelocity.isZero() ) // special case if new velocity is zero
		return;

	// use vector length to see if we exceed some aspect change limit
	// without overflowing on long vectors.

	MlScalar norm = localVelocity.length();
	MlScalar ac = localVelocity.dot(s->sm.forward);

	// compute the cosine of the angle
	MlScalar cosine = mlDiv(ac, norm);

	if ( ac < ML_SCALAR_ZERO || cosine < COSTHETA )
	{
		// use a linear approximation of cosine to get the delta
		MlScalar delta = mlDiv((COSTHETA - cosine), (ML_SCALAR_ONE - cosine));

		// rotate velocity by delta toward forward
		MlRotation rot0(ML_SCALAR_ZERO,ML_SCALAR_ZERO,ML_SCALAR_ZERO,ML_SCALAR_ONE);
		MlRotation rot1(localVelocity,s->sm.forward);
		MlRotation correction = MlRotation::slerp(
			rot0,
			rot1,
			delta);

		MlVector3 tmp;
		correction.multVec(localVelocity,tmp);
		s->sm.globalizeVector(tmp,velocity);

		// correct the acceleration to produce this velocity
		s->sm.acceleration = velocity - s->sm.velocity;
	}
}

