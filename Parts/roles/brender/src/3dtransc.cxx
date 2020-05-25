/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtransc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for managing a
 * transform on a Role.
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

// Include BRender target header files.
#include "math/scalarbr.h"
#include "math/transfrm.h"
#include "mle/br3drole.h"
#include "mle/3dtransc.h"


int Mle3dTransformCarrier::set(MleRole *role, MlTransform transform)
{
	int result = FALSE;

    br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
    if (root)
    {
        root->t.type = BR_TRANSFORM_MATRIX34;
        root->t.t.mat.m[0][0] = mlScalarToBRScalar(transform[0][0]);
        root->t.t.mat.m[0][1] = mlScalarToBRScalar(transform[0][1]);
        root->t.t.mat.m[0][2] = mlScalarToBRScalar(transform[0][2]);
        root->t.t.mat.m[1][0] = mlScalarToBRScalar(transform[1][0]);
        root->t.t.mat.m[1][1] = mlScalarToBRScalar(transform[1][1]);
        root->t.t.mat.m[1][2] = mlScalarToBRScalar(transform[1][2]);
        root->t.t.mat.m[2][0] = mlScalarToBRScalar(transform[2][0]);
        root->t.t.mat.m[2][1] = mlScalarToBRScalar(transform[2][1]);
        root->t.t.mat.m[2][2] = mlScalarToBRScalar(transform[2][2]);
        root->t.t.mat.m[3][0] = mlScalarToBRScalar(transform[3][0]);
        root->t.t.mat.m[3][1] = mlScalarToBRScalar(transform[3][1]);
        root->t.t.mat.m[3][2] = mlScalarToBRScalar(transform[3][2]);
        result=TRUE;
    }

    return result;
}


int Mle3dTransformCarrier::get(MleRole *role, MlTransform &transform)
{
	int result = FALSE;

    br_actor* root =(br_actor*)(Mle3dRole::cast(role)->getRoot());
    if (root)
    {
        br_matrix34 mat;
        BrTransformToMatrix34(&mat,&root->t);

        transform[0][0] = mlBRScalarToScalar(mat.m[0][0]);
        transform[0][1] = mlBRScalarToScalar(mat.m[0][1]);
        transform[0][2] = mlBRScalarToScalar(mat.m[0][2]);
        transform[1][0] = mlBRScalarToScalar(mat.m[1][0]);
        transform[1][1] = mlBRScalarToScalar(mat.m[1][1]);
        transform[1][2] = mlBRScalarToScalar(mat.m[1][2]);
        transform[2][0] = mlBRScalarToScalar(mat.m[2][0]);
        transform[2][1] = mlBRScalarToScalar(mat.m[2][1]);
        transform[2][2] = mlBRScalarToScalar(mat.m[2][2]);
        transform[3][0] = mlBRScalarToScalar(mat.m[3][0]);
        transform[3][1] = mlBRScalarToScalar(mat.m[3][1]);
        transform[3][2] = mlBRScalarToScalar(mat.m[3][2]);
        result=TRUE;
    }

    return result;
}
