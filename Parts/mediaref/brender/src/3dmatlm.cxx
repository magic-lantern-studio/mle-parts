/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmatlm.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Material Media Reference
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
#include <windows.h>
#else //not WIN32
#include <unistd.h>
#endif

// Include Magic Lantern header files.
#include "mle/mlFileIO.h"
#include "math/scalarbr.h"
#include "math/vector.h"
#include "mle/3dmatlm.h"
#include "mle/brendapi.h"


MLE_MEDIAREF_SOURCE(Mle3dMaterialMedia,MleMediaRef);


Mle3dMaterialMedia::Mle3dMaterialMedia()
{
   buffer = NULL;
   materialCount = 0;
   materialList = NULL;
}


Mle3dMaterialMedia::~Mle3dMaterialMedia()
{
    // Do nothing.
}


// XXX This is a hokey max on loading BRender registries
#define MLE_MAX_MATERIALS 1024


br_material** Mle3dMaterialMedia::read()
{
    // Read BRender Material Registry from external reference
    if (buffer)
        materialList = loadMaterialRegistry((char*)buffer,materialCount);

    if (! (materialList && materialCount))
        fprintf(stderr,"Mle3dMaterialMedia::Warning--no materials found in material registry.\n");

    return materialList;
}


br_material** Mle3dMaterialMedia::loadMaterialRegistry(char* filename, int& numMaterials)
{
    br_material*  tempRegistry[MLE_MAX_MATERIALS];
    br_material** materialRegistry = NULL;
    numMaterials = 0;

    if (filename)
	{
        int filecheck = mlOpen(filename,O_RDONLY);
        if (filecheck > -1)
		{
			mlClose(filecheck);

            numMaterials = BrMaterialLoadMany(filename,tempRegistry,MLE_MAX_MATERIALS);

            if (numMaterials)
			{
				materialRegistry = new br_material*[numMaterials];
                for (int i = 0; i < numMaterials; i++)
				{
					materialRegistry[i] = tempRegistry[i];
                    materialRegistry[i]->flags = BR_MATF_SMOOTH | BR_MATF_LIGHT | BR_MATF_TWO_SIDED;
                    BrMaterialAdd(materialRegistry[i]);
				}
			}
            else
                fprintf(stderr,"Mle3dMaterialMedia::Error--Could not load materials from file %s.\n",filename);
		}
        else
            fprintf(stderr,"Mle3dMaterialMedia::Error--Could not find material registry file %s.\n",filename);
	}
    else
        fprintf(stderr,"Mle3dMaterialMedia::Error--No filename given for material registry file.\n");

    return materialRegistry;
}


br_material* mlBrGetMaterial(MlVector3 ambientColor, MlVector3 diffuseColor,
                             MlVector3 specularColor, MlScalar shininess, MlScalar transparency,
                             int lightingOn, int shadingSmooth,
                             int facesDoubleSided, int facesAlwaysInFront)
{
    br_material* myMaterial = BrMaterialAllocate("myMaterial");

    myMaterial->colour = BR_COLOUR_RGBA(mlScalarToBRScalar(diffuseColor[0] * 255),
                                        mlScalarToBRScalar(diffuseColor[1] * 255),
                                        mlScalarToBRScalar(diffuseColor[2] * 255),
                                        mlScalarToBRScalar((ML_SCALAR_ONE - transparency) * 255));

    MlScalar tambient = ambientColor[0] + ambientColor[1] + ambientColor[2];
    MlScalar tdiffuse = diffuseColor[0] + diffuseColor[1] + diffuseColor[2];
    MlScalar total = tambient+tdiffuse;

    if (total > ML_SCALAR_ZERO)
	{
		myMaterial->ka = BrScalarToUFraction(mlScalarToBRScalar(mlDiv(tambient,total)));
        myMaterial->kd = BrScalarToUFraction(mlScalarToBRScalar(mlDiv(tdiffuse,total)));
	}
    else
	{
		myMaterial->ka = BR_UFRACTION(0.0);
        myMaterial->kd = BR_UFRACTION(0.0);
	}

    myMaterial->ks = BrScalarToUFraction(mlScalarToBRScalar
				     ( specularColor[0]
				       +specularColor[1]
				       +mlDiv(specularColor[2],mlLongToScalar(3))));

    myMaterial->power=mlScalarToBRScalar(shininess*100);

    myMaterial->flags=0;

    if (lightingOn)
        myMaterial->flags |= BR_MATF_LIGHT;
    else
        myMaterial->flags |= BR_MATF_PRELIT;

    if (shadingSmooth)
        myMaterial->flags |= BR_MATF_SMOOTH;

    if (facesDoubleSided)
        myMaterial->flags |= BR_MATF_TWO_SIDED;
    else
        myMaterial->flags |= BR_MATF_ALWAYS_VISIBLE;

    if (facesAlwaysInFront)
        myMaterial->flags |= BR_MATF_FORCE_Z_0;

    BrMaterialAdd(myMaterial);

    return myMaterial;
}


br_uint_8 mlBrGetRenderStyle(MleRenderStyle renderStyle)
{
    br_uint_8 myRenderStyle;

    switch (renderStyle)
	{
	    case MLE_RENDER_STYLE_INHERIT:
           myRenderStyle = BR_RSTYLE_DEFAULT;
           break;
        case MLE_RENDER_STYLE_NONE:
           myRenderStyle = BR_RSTYLE_NONE;
           break;
        case MLE_RENDER_STYLE_POINTS:
           myRenderStyle = BR_RSTYLE_POINTS;
           break;
        case MLE_RENDER_STYLE_LINES:
           myRenderStyle = BR_RSTYLE_EDGES;
           break;
        case MLE_RENDER_STYLE_FACES:
           myRenderStyle = BR_RSTYLE_FACES;
           break;
        default:
           myRenderStyle = BR_RSTYLE_DEFAULT;
           break;
	}

    return myRenderStyle;
}

