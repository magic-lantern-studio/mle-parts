/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3drendc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Render property
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

#include "mle/brendapi.h"
#include "mle/br3drole.h"
#include "mle/3drendc.h"

// Forward declarations.
void mlBrSetRenderStyle(br_actor*,int,int,int,int,int);
void mlBrGetRenderStyle(br_actor*,int&,int&,int&,int&,int&);


int Mle3dRenderCarrier::set(MleRole* role, int style, int shading, 
                           int texturing, int textureBlending,
                           int boundingBox, int facesDoubleSided, int facesAlwaysInFront)
{
	int result = FALSE;

    br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
    if (root)
    {
	     switch (style)
		 {
		   case OFF:
             root->render_style = BR_RSTYLE_NONE;
             break;
           case FACES:
             root->render_style = BR_RSTYLE_FACES;
             break;
           case LINES:
             root->render_style = BR_RSTYLE_EDGES;
             break;
           case POINTS:
             root->render_style = BR_RSTYLE_POINTS;
             break;
           default:
             root->render_style = BR_RSTYLE_DEFAULT;
             break;
		 }

         if (boundingBox)
         {
		     switch (root->render_style)
             {
               case BR_RSTYLE_NONE:
                   break;
               case BR_RSTYLE_FACES:
                   root->render_style = BR_RSTYLE_BOUNDING_FACES;
                   break;
               case BR_RSTYLE_EDGES:
                   root->render_style = BR_RSTYLE_BOUNDING_EDGES;
                   break;
               case BR_RSTYLE_POINTS:
                   root->render_style = BR_RSTYLE_BOUNDING_POINTS;
                   break;
               default:
                   root->render_style = BR_RSTYLE_BOUNDING_FACES;
             }
         }

         mlBrSetRenderStyle(root,shading,texturing,textureBlending,
                            facesDoubleSided,facesAlwaysInFront);

         result = TRUE;
    }

    return result;
}


int Mle3dRenderCarrier::get(MleRole* role, int& style, int& shading, 
                           int& texturing, int& textureBlending,
                           int& boundingBox, int& facesDoubleSided, int& facesAlwaysInFront)
{
	int result = FALSE;

    br_actor* root=(br_actor*)(Mle3dRole::cast(role)->getRoot());
    if (root)
    {
	    switch (root->render_style)
		{
		  case BR_RSTYLE_NONE:
             style = OFF;
             boundingBox = FALSE;
             break;
          case BR_RSTYLE_FACES:
             style = FACES;
             boundingBox = FALSE;
             break;
          case BR_RSTYLE_EDGES:
             style = LINES;
             boundingBox = FALSE;
             break;
          case BR_RSTYLE_POINTS:
             style = POINTS;
             boundingBox = FALSE;
             break;
          case BR_RSTYLE_BOUNDING_FACES:
             style = FACES;
             boundingBox = TRUE;
             break;
          case BR_RSTYLE_BOUNDING_EDGES:
             style = LINES;
             boundingBox = TRUE;
             break;
          case BR_RSTYLE_BOUNDING_POINTS:
             style = POINTS;
             boundingBox = TRUE;
             break;
          default:
             style = FACES;
             boundingBox = FALSE;
             break;
		}
        result = TRUE;
    }


    mlBrGetRenderStyle(root,shading,texturing,textureBlending,
                       facesDoubleSided,facesAlwaysInFront);

    return result;
}


void mlBrGetRenderStyle(br_actor* /*root*/,int& /*shading*/,
                        int& /*texturing*/, int& /*textureBlending*/,
                        int& /*facesDoubleSided*/,int& /*facesAlwaysInFront*/)
{ //Not implemented yet

}


void mlBrSetRenderStyle(br_actor* root,int shading,
                        int texturing, int textureBlending,
                        int facesDoubleSided,int facesAlwaysInFront)
{
    if (root)
    {
        if (root->material)
        { 
            if (shading==0)
            { root->material->flags&=~BR_MATF_LIGHT;
              root->material->flags&=~BR_MATF_SMOOTH;
              BrMaterialUpdate(root->material,BR_MATU_LIGHTING);
            }
            else
            if (shading==1)
            { root->material->flags|=BR_MATF_LIGHT;
              root->material->flags|=BR_MATF_SMOOTH;
              BrMaterialUpdate(root->material,BR_MATU_LIGHTING);
            }
            else
            if (shading==2)
            { root->material->flags&=~BR_MATF_LIGHT;
              root->material->flags|=BR_MATF_SMOOTH;
              BrMaterialUpdate(root->material,BR_MATU_LIGHTING);
            }
            else
            if (shading==3)
            { root->material->flags|=BR_MATF_LIGHT;
              root->material->flags&=BR_MATF_SMOOTH;
              BrMaterialUpdate(root->material,BR_MATU_LIGHTING);
            }
               
            if (texturing)
            { ///XXX need to reload texture from the actor
            }
            else
            {
              root->material->colour_map=NULL;
              BrMaterialUpdate(root->material,BR_MATU_COLOURMAP);
            }

            if (textureBlending)
            { root->material->flags&=~BR_MATF_DECAL;
              BrMaterialUpdate(root->material,BR_MATU_RENDERING);
            }
            else
            { root->material->flags|=BR_MATF_DECAL;
              BrMaterialUpdate(root->material,BR_MATU_RENDERING);
            }
            if (facesDoubleSided)
            { root->material->flags|=BR_MATF_TWO_SIDED;
              root->material->flags|=BR_MATF_ALWAYS_VISIBLE;
            }
            else
            { root->material->flags&=~BR_MATF_TWO_SIDED;
              root->material->flags&=~BR_MATF_ALWAYS_VISIBLE;
            }

            if (facesAlwaysInFront)
                root->material->flags|=BR_MATF_FORCE_Z_0;
            else
                root->material->flags&=~BR_MATF_FORCE_Z_0;
          }         
                 

          mlBrSetRenderStyle(root->next,shading,texturing,textureBlending,
                             facesDoubleSided,facesAlwaysInFront);
          mlBrSetRenderStyle(root->children,shading,texturing,textureBlending,
                             facesDoubleSided,facesAlwaysInFront);
    }
                 
}
