/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dTransformCarrier.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for managing a transform
 * on a Role targeting the Inventor platrform.
 */

 // COPYRIGHT_BEGIN
  //
  // The MIT License (MIT)
  //
  // Copyright (c) 2000-2025 Wizzer Works
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


// Include Inventor target header files.
#include "Inventor/nodes/SoTransform.h"
#include "Inventor/nodes/SoSeparator.h"
#include "math/transfrm.h"
#include "mle/MleIv3dRole.h"
#include "mle/3dtransc.h"


int Mle3dTransformCarrier::set(MleRole* role, MlTransform transform)
{
    int result=FALSE;

    SoSeparator *root = Mle3dRole::cast(role)->getRoot();
    if (root)
    {
		SbMatrix ivmat;
		// Only filling in 3x4 of 4x4, so make it identity to start.
		ivmat.makeIdentity();
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 3; j++)
				ivmat[i][j] = mlScalarToFloat(transform[i][j]);
		}

		// First child is always transform.
		SoTransform *ivTransformNode = (SoTransform *) root->getChild(0);
		if (!ivTransformNode ||
			!ivTransformNode->isOfType(SoTransform::getClassTypeId()))
		{
			fprintf(stderr,"MleIv3dTransformCarrier::Error--first child isn't xform\n");
			return FALSE;
		}

		ivTransformNode->setMatrix(ivmat);
		result=TRUE;
    }

    return result;
}


int Mle3dTransformCarrier::get(MleRole* delegate, MlTransform& transform)
{ 
    int result=FALSE;

    SoSeparator *root = Mle3dRole::cast(delegate)->getRoot();
    if (root)
    {
		SoTransform *ivTransformNode = (SoTransform *) root->getChild(0);
		if (!ivTransformNode ||
			!ivTransformNode->isOfType(SoTransform::getClassTypeId()))
		{
			fprintf(stderr,"MleIv3dTransformCarrier::Error--first child isn't xform\n");
			return FALSE;
		}

		SbVec3f translation = ivTransformNode->translation.getValue();
		MlVector3 mlTranslation(mlFloatToScalar(translation[0]), 
			mlFloatToScalar(translation[1]), 
			mlFloatToScalar(translation[2]));

		SbRotation rot = ivTransformNode->rotation.getValue();
		float q0, q1, q2, q3;
		rot.getValue(q0, q1, q2, q3);
		MlRotation mlRot(mlFloatToScalar(q0), mlFloatToScalar(q1), 
			 mlFloatToScalar(q2), mlFloatToScalar(q3));

		SbVec3f scale = ivTransformNode->scaleFactor.getValue();
		MlVector3 mlScaleFactor(mlFloatToScalar(scale[0]), 
			mlFloatToScalar(scale[1]), 
			mlFloatToScalar(scale[2]));

		SbRotation scaleOrientation = 
			ivTransformNode->scaleOrientation.getValue();
		scaleOrientation.getValue(q0, q1, q2, q3);
		MlRotation mlScaleOrientation(mlFloatToScalar(q0), 
			mlFloatToScalar(q1), 
			mlFloatToScalar(q2), 
			mlFloatToScalar(q3));

		SbVec3f center = ivTransformNode->center.getValue();
		MlVector3 mlCenter(mlFloatToScalar(center[0]), 
			mlFloatToScalar(center[1]), 
			mlFloatToScalar(center[2]));

		transform.setTransform(mlTranslation, mlRot, mlScaleFactor,
			mlScaleOrientation, mlCenter);

		result = TRUE;
	}

    return result;
}

