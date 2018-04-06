/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dRenderStykeCarrier.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Render Style property
 * targeting the Inventor platform.
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

#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoDrawStyle.h"

#include "mle/MleIv3dRole.h"
#include "mle/3drstylp.h"
#include "mle/3drstylc.h"


int Mle3dRenderStyleCarrier::set(MleRole* role, int renderStyle)
{ int result = FALSE;

  SoSeparator* root = (SoSeparator*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     { SoNode* node = NULL;
       //XXX This should be modified to search for the render style node.
       if (node = root->getChild(1))
          { SoDrawStyle* styleNode=NULL;
            if (node->isOfType(SoDrawStyle::getClassTypeId()))
               styleNode = (SoDrawStyle*)node;
            else
               { styleNode = new SoDrawStyle;
                 root->insertChild(styleNode,1);
               }
            switch (renderStyle)
              { case Mle3dRenderStyleProperty::OFF:
                     styleNode->style.setValue(SoDrawStyle::INVISIBLE);
                     break;
                case Mle3dRenderStyleProperty::FACES:
                     styleNode->style.setValue(SoDrawStyle::FILLED);
                     break;
                case Mle3dRenderStyleProperty::LINES:
                     styleNode->style.setValue(SoDrawStyle::LINES);
                     break;
                case Mle3dRenderStyleProperty::POINTS:
                     styleNode->style.setValue(SoDrawStyle::POINTS);
                     break;
                default:
                     styleNode->style.setValue(SoDrawStyle::FILLED);
                     break;
              }
         }
       result=TRUE;
     }

  return result;
}


int Mle3dRenderStyleCarrier::get(MleRole* role, int& renderStyle)
{ int result = FALSE;

  SoSeparator* root = (SoSeparator*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     { SoNode* node = NULL;
       //XXX This should be modified to search for the render style node.
       if (node = root->getChild(1))
          { SoDrawStyle* styleNode=NULL;
            if (node->isOfType(SoDrawStyle::getClassTypeId()))
               styleNode = (SoDrawStyle*)node;
            if (styleNode)
               { if (styleNode->style.getValue()==SoDrawStyle::INVISIBLE)
                    renderStyle = Mle3dRenderStyleProperty::OFF;
                 else
                 if (styleNode->style.getValue()==SoDrawStyle::FILLED)
                    renderStyle = Mle3dRenderStyleProperty::FACES;
                 else
                 if (styleNode->style.getValue()==SoDrawStyle::LINES)
                    renderStyle = Mle3dRenderStyleProperty::LINES;
                 else
                 if (styleNode->style.getValue()==SoDrawStyle::POINTS)
                    renderStyle = Mle3dRenderStyleProperty::POINTS;
                 else
                    renderStyle = Mle3dRenderStyleProperty::FACES;
               }
          }
       result=TRUE;
     }

  return result;
}

