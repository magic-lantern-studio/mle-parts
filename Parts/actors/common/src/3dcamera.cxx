/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcama.h
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Camera Actor.
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

#include <mle/mlAssert.h>
#include <mle/3dcamera.h>
#include <mle/3dcamc.h>
#include <mle/MleSet.h>

MLE_ACTOR_SOURCE(My3dCameraActor,MleActor);

// constructor
My3dCameraActor::My3dCameraActor()
{ 
  // set initial values
  cameraType = 0;    // perspective camera

  position.m_translation.setValue(ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(0));
  orientation.m_rotation.setValue(ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(1));

  clipping.m_clipping[0] = ML_SCALAR(0.1);
  clipping.m_clipping[1] = ML_SCALAR(1000.0);

  // An aspect ratio of zero is a special value that indicates
  //   that the ratio should be taken from the screen aspect
  aspectRatio = ML_SCALAR(0);

  fieldOfView = ML_SCALAR(60.0);  
}


#ifdef MLE_REHEARSAL
void My3dCameraActor::initClass()
{
  // register the actor class
  mlRegisterActorClass(My3dCameraActor,MleActor);

  // register properties
  mlRegisterActorMember(My3dCameraActor,cameraType,int);
  mlRegisterActorMember(My3dCameraActor,position,MlVector3);
  mlRegisterActorMember(My3dCameraActor,orientation,MlRotation);
  mlRegisterActorMember(My3dCameraActor,clipping,MlVector2);
  mlRegisterActorMember(My3dCameraActor,aspectRatio,MlScalar);
  mlRegisterActorMember(My3dCameraActor,fieldOfView,MlScalar);

  //
  // Mark all the properties that belongs to the "transform" property
  // data set.
  //
  mlRegisterActorMemberDataset(My3dCameraActor,position,MLE_PROP_DATASET_TRANSFORM);
  mlRegisterActorMemberDataset(My3dCameraActor,orientation,MLE_PROP_DATASET_TRANSFORM);
}
#endif

#ifdef MLE_REHEARSAL
void My3dCameraActor::resolveEdit(const char* property)
{
  if (property)
     { if ( !strcmp(property,"position") )
      position.push(this);
       else 
       if ( !strcmp(property,"orientation") )
      orientation.push(this);
       else 
       if ( !strcmp(property,"clipping") )
            clipping.push(this);
       else 
       if ( !strcmp(property,"aspectRatio") )
            aspectRatio.push(this);
       else 
       if ( !strcmp(property,"fieldOfView") )
            fieldOfView.push(this);
     }
   else  
     update();
}
#endif /* MLE_REHEARSAL */

void My3dCameraActor::init()
{ 
  // send the current property values to the forum
  update();
}


void My3dCameraActor::update(void)
{
   // push all camera state to the forum
   position.push(this);
   orientation.push(this);
   clipping.push(this);
   aspectRatio.push(this);
   fieldOfView.push(this);
}
