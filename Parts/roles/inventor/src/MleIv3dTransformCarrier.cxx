/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dTransformCarrier.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for managing a transform
 * on a Role targeting the Inventor platrform.
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

