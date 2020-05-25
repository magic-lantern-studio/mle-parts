/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dRenderCarrier.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Render property
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

#include "Inventor/actions/SoSearchAction.h"
#include "Inventor/nodes/SoComplexity.h"
#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoDrawStyle.h"
#include "Inventor/nodes/SoShapeHints.h"
#include "Inventor/nodes/SoTexture2.h"

#include "mle/MleIv3dRole.h"
#include "mle/3drendp.h"
#include "mle/3drendc.h"


int Mle3dRenderCarrier::set(MleRole* role, int style, int shading, 
                            int texturing, int textureBlending, int boundingBox, 
                            int facesDoubleSided, int facesAlwaysInFront)
{ int result = FALSE;

  SoSeparator* root = (SoSeparator*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     { 
       //XXX This should be modified to search for the render style node.
       SoDrawStyle* styleNode = NULL;
       SoShapeHints* shapeNode = NULL;
       SoComplexity* complexityNode = NULL;
       if (root->getNumChildren()<=3) 
          { styleNode=  new SoDrawStyle;
            styleNode->ref();
            root->addChild(styleNode);
            shapeNode = new SoShapeHints;
            shapeNode->ref();
            root->addChild(shapeNode);
            complexityNode = new SoComplexity;
            complexityNode->ref();
            root->addChild(complexityNode);
          }

       SoNode* node;
       SoSearchAction search;

       node = NULL;
       if (!styleNode)
          { search.setFind(SoSearchAction::TYPE);
            search.setInterest(SoSearchAction::LAST);

            // Handle "style" flag.
            search.setType(SoDrawStyle::getClassTypeId(),TRUE);
            search.apply(root);
          
            if (search.getPath()) 
               node=search.getPath()->getTail();
          }

       if (node && node->isOfType(SoDrawStyle::getClassTypeId()))
          styleNode = (SoDrawStyle*)node;
       else
          { styleNode = new SoDrawStyle; 
            styleNode->ref();
            root->insertChild(styleNode,1);
          }

       if (styleNode)
          switch (style)
                 { case OFF:
                        styleNode->style.setValue(SoDrawStyle::INVISIBLE);
                        break;
                   case FACES:
                        styleNode->style.setValue(SoDrawStyle::FILLED);
                        break;
                   case LINES:
                        styleNode->style.setValue(SoDrawStyle::LINES);
                        break;
                   case POINTS:
                        styleNode->style.setValue(SoDrawStyle::POINTS);
                        break;
                   default:
                        styleNode->style.setValue(SoDrawStyle::FILLED);
                        break;
                 }
            

       // Handle "facesDoubleSided" flag.
       node = NULL;
       if (!shapeNode)
          { search.setType(SoShapeHints::getClassTypeId(),TRUE);
            search.apply(root);
            if (search.getPath())
               node = search.getPath()->getTail();
          }

       if (node && node->isOfType(SoShapeHints::getClassTypeId()))
          shapeNode=(SoShapeHints*)node;
       else
          { shapeNode = new SoShapeHints; 
            shapeNode->ref();
            root->insertChild(shapeNode,2);
          }

       if (shapeNode)
          {
            if (facesDoubleSided)
               { shapeNode->shapeType.setValue(SoShapeHints::UNKNOWN_SHAPE_TYPE);
                 shapeNode->vertexOrdering.setValue(SoShapeHints::UNKNOWN_ORDERING);
               }
            else
               { shapeNode->shapeType.setValue(SoShapeHints::SOLID);
                 shapeNode->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);
               }
 
            if (shading)
               shapeNode->creaseAngle.setValue(60);
            else
               shapeNode->creaseAngle.setValue(0);
          }    


       // Handle "texture" flag.
       node=NULL;
       if (!complexityNode)
          { search.setType(SoComplexity::getClassTypeId(),TRUE);
            search.apply(root);
            if (search.getPath())
               node=search.getPath()->getTail();
          }

       if (node && node->isOfType(SoComplexity::getClassTypeId()))
          complexityNode = (SoComplexity*)node;
       else
          { complexityNode = new SoComplexity; 
            complexityNode->ref();
            root->insertChild(complexityNode,3);
          }
       if (complexityNode)
          {
            if (texturing)
               complexityNode->textureQuality.setValue(1);
            else
               complexityNode->textureQuality.setValue(0);
          }

       // Handle "textureBlending" flag.
       search.setFind(SoSearchAction::TYPE);
       search.setInterest(SoSearchAction::ALL);
       search.setType(SoTexture2::getClassTypeId(),TRUE);
       search.apply(root);
       SoPathList found = search.getPaths();
       for (int i = 0; i < found.getLength(); i++)
           { node = found[i]->getTail();
             if (node && node->isOfType(SoTexture2::getClassTypeId()))
                { SoTexture2* textureNode = (SoTexture2*)node;
                  if (textureBlending == 0)
                     textureNode->model.setValue(SoTexture2::DECAL);
                  else
                  if (textureBlending == 1)
                     textureNode->model.setValue(SoTexture2::MODULATE);
                  else
                  if (textureBlending == 2)
                     textureNode->model.setValue(SoTexture2::BLEND);
                }
          }


       result = TRUE;
     }

  return result;
}


int Mle3dRenderCarrier::get(MleRole* role, int& style, int& /*shading*/, 
                            int& /*texturing*/, int& /*textureBlending*/, int& /*boundingBox*/, 
                            int& /*facesDoubleSided*/, int& /*facesAlwaysInFront*/)
{ int result=FALSE;

  SoSeparator* root = (SoSeparator*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     { SoNode* node = NULL;
       //XXX This should be modified to search for the render style node.
       if ((root->getNumChildren()>1) && (node=root->getChild(1)))
          { SoDrawStyle* styleNode = NULL;
            if (node->isOfType(SoDrawStyle::getClassTypeId()))
               styleNode = (SoDrawStyle*)node;
            if (styleNode)
               { if (styleNode->style.getValue() == SoDrawStyle::INVISIBLE)
                    style = OFF;
                 else
                 if (styleNode->style.getValue() == SoDrawStyle::FILLED)
                    style = FACES;
                 else
                 if (styleNode->style.getValue() == SoDrawStyle::LINES)
                    style = LINES;
                 else
                 if (styleNode->style.getValue() == SoDrawStyle::POINTS)
                    style = POINTS;
                 else
                    style = FACES;
               }
          }
       result=TRUE;
     }

  return result;
}

