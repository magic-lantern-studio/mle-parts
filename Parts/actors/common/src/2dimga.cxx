/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dimga.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 2D Image Actor.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2025 Wizzer Works
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

#include "mle/mlMalloc.h"

// Include Magic Lantern Parts header files.
#include "mle/2dimga.h"
#include "mle/2dimgd.h"


MLE_ACTOR_SOURCE(Mle2dImgActor, MleActor);


#ifdef MLE_REHEARSAL
void
Mle2dImgActor::initClass(void)
{
    mleRegisterActorClass(Mle2dImgActor, MleActor);

    mleRegisterActorMember(Mle2dImgActor, position, MlVector2);
    mleRegisterActorMember(Mle2dImgActor, image, MlMediaRef);
    mleRegisterActorMember(Mle2dImgActor, displayState, int);

    //
    // Mark all the properties that belongs to the "transform" property
    // data set.
    //
    mleRegisterActorMemberDataset(Mle2dImgActor, position, MLE_PROP_DATASET_TRANSFORM);
}
#endif /* MLE_REHEARSAL */


Mle2dImgActor::Mle2dImgActor(void)
 : MleActor()
{
    //
    // Set default property values.
    //
    position.setValue(ML_SCALAR(0), ML_SCALAR(0));
    image = MLE_NO_MEDIA;
    displayState = FALSE;
    m_mref = NULL;
}


Mle2dImgActor::~Mle2dImgActor(void)
{
}


void
Mle2dImgActor::init(void)
{
    Mle2dImgRole *role = Mle2dImgRole::cast(getRole());

    //
    // Push initial data got from the workprint to the role.
    //
    role->screenLocation(position);
    if (image != MLE_NO_MEDIA) role->load(image); // Load from Digital Playprint.
    else if (m_mref != NULL) role->load(m_mref); // Load from registered Media Reference.
    role->display(displayState);
}


#ifdef MLE_REHEARSAL
void
Mle2dImgActor::resolveEdit(const char *property)
{
    Mle2dImgRole *role = Mle2dImgRole::cast(getRole());

    if (! property || strcmp(property, "position") == 0)
	    role->screenLocation(position);
    else if(strcmp(property, "image") == 0)
	    role->load(image);
    else if(strcmp(property, "displayState") == 0)
	    role->display(displayState);
}
#endif /* MLE_REHEARSAL */


void
Mle2dImgActor::setPosition(MlVector2 &pos)
{
    Mle2dImgRole *role = Mle2dImgRole::cast(getRole());

    //
    // Update position property and push the update to the role.
    //
    position.setValue(pos.getValue());
    role->screenLocation(position);
}


void
Mle2dImgActor::setImage(MlMediaRef img)
{
    Mle2dImgRole *role = Mle2dImgRole::cast(getRole());

    //
    // Update image property and push the update to the role.
    //
    image = img;
    m_mref = NULL;
    role->load(img);
}


void
Mle2dImgActor::setImage(MleMediaRef *img)
{
    Mle2dImgRole *role = Mle2dImgRole::cast(getRole());

    //
    // Update image property and push the update to the role.
    //
    image = MLE_NO_MEDIA;
    m_mref = img;
    role->load(m_mref);
}


void
Mle2dImgActor::setVisible(int state)
{
    Mle2dImgRole *role = Mle2dImgRole::cast(getRole());

    //
    // Update displayState property and push the update to the role.
    //
    displayState = state;
    role->display(state);
}


int
Mle2dImgActor::getVisible(void)
{
    return displayState;
}


void
Mle2dImgActor:: getProperty(MleObject *object, const char *name, unsigned char **value)
{
    //cout << "Getting Mle2dImgActor property " << name << "." << endl;
    if (strcmp("position",name) == 0)
    {
        MlVector2 property = ((Mle2dImgActor *)object)->getPositionProperty();
        *((MlVector2 *)value) = property;
    } else if (strcmp("image",name) == 0)
    {
        MlMediaRef property = ((Mle2dImgActor *)object)->getImageProperty();
        *((MlMediaRef *)value) = property;
    } else if (strcmp("displayState",name) == 0)
    {
        int property = ((Mle2dImgActor *)object)->getDisplayStateProperty();
        *((int *)value) = property;
    } else
    {
        // TBD: log warning.
        //cout << "***** ERROR: Unknown Mle2dImgActor property: " << name << endl;
    }
}


void
Mle2dImgActor::setProperty(MleObject *object, const char *name, unsigned char *value)
{
    //cout << "Setting Mle2dImgActor property " << name << "." << endl;
    if (strcmp("position",name) == 0)
    {
        ((Mle2dImgActor *)object)->setPositionProperty(*((MlVector2 *)value));
    } else if (strcmp("image",name) == 0)
    {
        ((Mle2dImgActor *)object)->setImageProperty(*((MlMediaRef *)value));
    } else if (strcmp("displayState",name) == 0)
    {
        ((Mle2dImgActor *)object)->setDisplayStateProperty(*((int *)value));
    } else {
        // TBD: log warning.
        //cout << "***** ERROR: Unknown Mle2dImgActor property: " << name << endl;
    }
}

void*
Mle2dImgActor::operator new(size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void*
Mle2dImgActor::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
Mle2dImgActor::operator delete(void* p)
{
    mlFree(p);
}

void
Mle2dImgActor::operator delete[](void* p)
{
    mlFree(p);
}
