/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3drstylc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Render Style property
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
#include "mle/3drstylp.h"
#include "mle/3drstylc.h"


int Mle3dRenderStyleCarrier::set(MleRole* role, int renderStyle)
{ int result=FALSE;

  br_actor* root=(br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     { switch (renderStyle)
              { case Mle3dRenderStyleProperty::OFF:
                     root->render_style = BR_RSTYLE_NONE;
                     break;
                case Mle3dRenderStyleProperty::FACES:
                     root->render_style = BR_RSTYLE_FACES;
                     break;
                case Mle3dRenderStyleProperty::LINES:
                     root->render_style = BR_RSTYLE_EDGES;
                     break;
                case Mle3dRenderStyleProperty::POINTS:
                     root->render_style = BR_RSTYLE_POINTS;
                     break;
                default:
                     root->render_style = BR_RSTYLE_DEFAULT;
                     break;
              }
       result=TRUE;
     }

  return result;
}


int Mle3dRenderStyleCarrier::get(MleRole* role, int& renderStyle)
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     { switch (root->render_style)
              { case BR_RSTYLE_NONE:
                     renderStyle = Mle3dRenderStyleProperty::OFF;
                     break;
                case BR_RSTYLE_FACES:
                     renderStyle = Mle3dRenderStyleProperty::FACES;
                     break;
                case BR_RSTYLE_EDGES:
                     renderStyle = Mle3dRenderStyleProperty::LINES;
                     break;
                case BR_RSTYLE_POINTS:
                     renderStyle = Mle3dRenderStyleProperty::POINTS;
                     break;
              }
       result = TRUE;
     }

  return result;
}
