/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcama.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Camera Actor.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2019 Wizzer Works
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
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include Magic Lantern header files.
#include <mle/mlAssert.h>

// Include Magic Lantern Runtime Engine header files.
#include <mle/MleSet.h>

// Inlucde Magic Lantern Extension header files.
#include <mle/3dcama.h>
#include <mle/3dcamc.h> // For the role interface.

// Inlclude Magic Lantern Math header files.
#include <math/angle.h>
#include <math/scalar.h>


MLE_ACTOR_SOURCE(Mle3dCameraActor,MleActor);


// Constructor.
Mle3dCameraActor::Mle3dCameraActor()
{ 
	// Set initial values.
	position.setValue(ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(0));
	orientation.setValue(ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(1));

	nearClipping = ML_SCALAR(0.1);
	farClipping = ML_SCALAR(1000.0);

	// An aspect ratio of zero is a special value that indicates
	//   that the ratio should be taken from the screen aspect.
	aspectRatio = ML_SCALAR(0);

	cameraType = 0;  //Perspective camera.
	viewHeight = ML_SCALAR_PI_FOURTH;  // PI/4
}


#ifdef MLE_REHEARSAL
void Mle3dCameraActor::initClass()
{
	// Register the actor class.
	mleRegisterActorClass(Mle3dCameraActor,MleActor);

	// Register properties.
	mleRegisterActorMember(Mle3dCameraActor,position,MlVector3);
	mleRegisterActorMember(Mle3dCameraActor,orientation,MlRotation);
	mleRegisterActorMember(Mle3dCameraActor,nearClipping,MlScalar);
	mleRegisterActorMember(Mle3dCameraActor,farClipping,MlScalar);
	mleRegisterActorMember(Mle3dCameraActor,aspectRatio,MlScalar);
	mleRegisterActorMember(Mle3dCameraActor,cameraType,int);
	mleRegisterActorMember(Mle3dCameraActor,viewHeight,MlScalar);

	//
	// Mark all the properties that belongs to the "transform" property
	// data set.
	//
	mleRegisterActorMemberDataset(Mle3dCameraActor,position,MLE_PROP_DATASET_TRANSFORM);
	mleRegisterActorMemberDataset(Mle3dCameraActor,orientation,MLE_PROP_DATASET_TRANSFORM);
}
#endif /* MLE_REHEARSAL */

#ifdef MLE_REHEARSAL
void
Mle3dCameraActor::resolveEdit(const char *property)
{
	if (property)
	{
	    if ( 0 )
		    ;
	    else if ( !strcmp(property,"position") )
		    Mle3dCameraCarrier::setPosition(m_set,&position);
	    else if ( !strcmp(property,"orientation") )
		    Mle3dCameraCarrier::setOrientation(m_set,&orientation);
	    else if ( !strcmp(property,"nearClipping") )
		    Mle3dCameraCarrier::setNearClipping(m_set,nearClipping);
	    else if ( !strcmp(property,"farClipping") )
		    Mle3dCameraCarrier::setFarClipping(m_set,farClipping);
	    else if ( !strcmp(property,"aspectRatio") )
		    Mle3dCameraCarrier::setAspectRatio(m_set,aspectRatio);
	    else
		    update();
	}
	else
		update();
}
#endif /* MLE_REHEARSAL */

void
Mle3dCameraActor::init()
{ 
	// Remember the Set this actor is created for.
	m_set = MleSet::g_currentSet;

	// Send the current property values to the Set.
	update();
}

void
Mle3dCameraActor::setPosition(const MlVector3 *p,MlBoolean pushToSet)
{
	// Set the property value.
	position = *p;

	// Push state to the Set.
	if ( pushToSet == TRUE )
		Mle3dCameraCarrier::setPosition(m_set,&position);
}

void
Mle3dCameraActor::getPosition(MlVector3 *p)
{
	MLE_ASSERT(p);

	// Set the return value from the local state.  Note that this
	//   is not necessarily the parameter of the current view if
	//   other cameras are being used in the same Set.
	*p = position;
}

void
Mle3dCameraActor::setOrientation(const MlRotation *o,MlBoolean pushToSet)
{
	// Set the property value.
	orientation = *o;

	// Push state to the Set.
	if ( pushToSet == TRUE )
		Mle3dCameraCarrier::setOrientation(m_set,&orientation);
}

void
Mle3dCameraActor::getOrientation(MlRotation *o)
{
	MLE_ASSERT(o);

	// Set the return value from the local state.  Note that this
	//   is not necessarily the parameter of the current view if
	//   other cameras are being used in the same Set.
	*o = orientation;
}

void
Mle3dCameraActor::setNearClipping(MlScalar n,MlBoolean pushToSet)
{
	// Set the property value.
	nearClipping = n;

	// Push state to the Set.
	if ( pushToSet == TRUE )
		Mle3dCameraCarrier::setNearClipping(m_set,nearClipping);
}

MlScalar
Mle3dCameraActor::getNearClipping(void)
{
	// Set the return value from the local state.  Note that this
	//   is not necessarily the parameter of the current view if
	//   other cameras are being used in the same Set.
	return nearClipping;
}

void
Mle3dCameraActor::setFarClipping(MlScalar f,MlBoolean pushToSet)
{
	// Set the property value.
	farClipping = f;

	// Push state to the Set.
	if ( pushToSet == TRUE )
		Mle3dCameraCarrier::setFarClipping(m_set,farClipping);
}

MlScalar
Mle3dCameraActor::getFarClipping(void)
{
	// Set the return value from the local state.  Note that this
	//   is not necessarily the parameter of the current view if
	//   other cameras are being used in the same Set.
	return farClipping;
}

void
Mle3dCameraActor::setAspectRatio(MlScalar a,MlBoolean pushToSet)
{
	// set the property value.
	aspectRatio = a;

	// Push state to the Set.
	if ( pushToSet == TRUE )
		Mle3dCameraCarrier::setAspectRatio(m_set,aspectRatio);
}

MlScalar
Mle3dCameraActor::getAspectRatio(void)
{
	// Set the return value from the local state.  Note that this
	//   is not necessarily the parameter of the current view if
	//   other cameras are being used in the same Set.
	return aspectRatio;
}

void
Mle3dCameraActor::setCameraType(int c,MlBoolean pushToSet)
{
	// Set the property value.
	cameraType = c;

	// This is not communicated by a carrier immediately.
}

int
Mle3dCameraActor::getCameraType(void)
{
	// Set the return value from the local state.  Note that this
	//   is not necessarily the parameter of the current view if
	//   other cameras are being used in the same Set.
	return cameraType;
}

void
Mle3dCameraActor::setViewHeight(MlScalar v,MlBoolean pushToSet)
{
	// Set the property value.
	viewHeight = v;

	// Push state to the Set.
	if ( pushToSet == TRUE )
	{
		if ( cameraType == 0 )
			// Perspective camera.
			Mle3dCameraCarrier::setPerspectiveFieldOfView(
				m_set,viewHeight);
		else
			// Orthographic camera.
			Mle3dCameraCarrier::setOrthographicViewHeight(
				m_set,viewHeight);
	}
}

MlScalar
Mle3dCameraActor::getViewHeight(void)
{
	// Set the return value from the local state.  Note that this
	//   is not necessarily the parameter of the current view if
	//   other cameras are being used in the same Set.
	return viewHeight;
}

void
Mle3dCameraActor::update(void)
{
	// Push all camera state to the Set.
	Mle3dCameraCarrier::setPosition(m_set,&position);
	Mle3dCameraCarrier::setOrientation(m_set,&orientation);
	Mle3dCameraCarrier::setNearClipping(m_set,nearClipping);
	Mle3dCameraCarrier::setFarClipping(m_set,farClipping);
	Mle3dCameraCarrier::setAspectRatio(m_set,aspectRatio);

	switch ( cameraType )
	{
	    case 0:	// Perspective
			Mle3dCameraCarrier::setPerspectiveFieldOfView(
				m_set,viewHeight);
			break;
	    case 1:	// Orthographic
			Mle3dCameraCarrier::setOrthographicViewHeight(
				m_set,viewHeight);
			break;
	}
}

void
Mle3dCameraActor:: getProperty(MleObject *object, const char *name, unsigned char **value)
{
    //cout << "Getting Mle3dCameraActor property " << name << "." << endl;
    if (strcmp("position",name) == 0)
    {
        MlVector3 property = ((Mle3dCameraActor *)object)->getPositionProperty();
        *((MlVector3 *)value) = property;
    } else if (strcmp("orientation",name) == 0)
    {
        MlRotation property = ((Mle3dCameraActor *)object)->getOrientationProperty();
        *((MlRotation *)value) = property;
    } else if (strcmp("nearClipping",name) == 0)
    {
        MlScalar property = ((Mle3dCameraActor *)object)->getNearClippingProperty();
        *((MlScalar *)value) = property;
    } else if (strcmp("farClipping",name) == 0)
    {
        MlScalar property = ((Mle3dCameraActor *)object)->getFarClippingProperty();
        *((MlScalar *)value) = property;
    } else if (strcmp("aspectRatio",name) == 0)
    {
        MlScalar property = ((Mle3dCameraActor *)object)->getAspectRatioProperty();
        *((MlScalar *)value) = property;
    } else if (strcmp("cameraType",name) == 0)
    {
        int property = ((Mle3dCameraActor *)object)->getCameraTypeProperty();
        *((int *)value) = property;
    } else if (strcmp("viewType",name) == 0)
    {
        MlScalar property = ((Mle3dCameraActor *)object)->getViewHeightProperty();
        *((MlScalar *)value) = property;
    } else
    {
        // TBD: log warning.
        //cout << "***** ERROR: Unknown Mle3dCameraActor property: " << name << endl;
    }
}

void
Mle3dCameraActor::setProperty(MleObject *object, const char *name, unsigned char *value)
{
    //cout << "Setting Mle3dCameraActor property " << name << "." << endl;
    if (strcmp("position",name) == 0)
    {
        ((Mle3dCameraActor *)object)->setPositionProperty(*((MlVector3 *)value));
    } else if (strcmp("orientation",name) == 0)
    {
        ((Mle3dCameraActor *)object)->setOrientationProperty(*((MlRotation *)value));
    } else if (strcmp("nearClipping",name) == 0)
    {
        ((Mle3dCameraActor *)object)->setNearClippingProperty(*((MlScalar *)value));
    } else if (strcmp("farClipping",name) == 0)
    {
        ((Mle3dCameraActor *)object)->setFarClippingProperty(*((MlScalar *)value));
    } else if (strcmp("aspectRatio",name) == 0)
    {
        ((Mle3dCameraActor *)object)->setAspectRatioProperty(*((MlScalar *)value));
    } else if (strcmp("cameraType",name) == 0)
    {
        ((Mle3dCameraActor *)object)->setCameraTypeProperty(*((int *)value));
    } else if (strcmp("viewHeight",name) == 0)
    {
        ((Mle3dCameraActor *)object)->setViewHeightProperty(*((MlScalar *)value));
    } else {
        // TBD: log warning.
        //cout << "***** ERROR: Unknown Mle3dCameraActor property: " << name << endl;
    }
}
