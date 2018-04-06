/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dimgrole.cpp
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Image Role targeting
 * the Qt platform.
 *
 * @author Mark S. Millard
 * @date Jul 8, 2017
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017 Wizzer Works
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

// Include system headre files.
#include <stdlib.h>

// Include Magic Lantern header files.
#include "mle/MleActor.h"
#include "mle/MleLoad.h"

// Include Magic Lantern Qt platform header files.
#include "2dimgrole.h"
#include "mle/qt/qtstage.h"
#include "mle/qt/imagmref.h"


MLE_ROLE_SOURCE(Mle2dImgRole, Mle2dRole)


Mle2dImgRole::Mle2dImgRole(MleActor *actor)
  : Mle2dRole(actor)
{
    // Do nothing extra.
}

Mle2dImgRole::~Mle2dImgRole()
{
    screenPosition.setValue(ML_SCALAR(0), ML_SCALAR(0));
    displayState = FALSE;

   // Get the Qt stage for the associated device.
   QtStage *theStage = QtStage::cast(MleStage::g_theStage);

   // Create a new Magic Lantern surface and initialize it.
   //m_surface = new QPixmap();
}

void
Mle2dImgRole::screenLocation(MlVector2 &pos)
{
     screenPosition.setValue(pos.getValue());
}

void
Mle2dImgRole::load(MlMediaRef img)
{
    MleImageMediaRef *imageMediaRef;

    imageMediaRef = (MleImageMediaRef *) mlLoadMediaRef(img, NULL);

    if (imageMediaRef)
    {
        // Remember the Media Reference identifier.
        m_imgMediaRef = img;

        // Read the image data into the supplied surface.
        imageMediaRef->read(&m_surface);

        // Register with the stage to free up resources when Qt resets.
        //QtStage *theStage = QtStage::cast(MleStage::g_theStage);
        //theStage->addResetCallback(Mle2dImgRole::onReset, this);
    } else
    {
        m_imgMediaRef = MLE_NO_MEDIA;
    }
}

void
Mle2dImgRole::load(MleMediaRef *img)
{
    MleImageMediaRef *imageMediaRef;

    imageMediaRef = (MleImageMediaRef *) img;

    // ToDo: validate img.
    if (img)
    {
        // Remember the Media Reference identifier.
        //m_imgMediaRef = ???;

        // Read the image data into the supplied surface.
        imageMediaRef->read(&m_surface);

        // Register with the stage to free up resources when Qt resets.
        //QtStage *theStage = QtStage::cast(MleStage::g_theStage);
        //theStage->addResetCallback(Mle2dImgRole::onReset, this);
    } else
    {
        m_imgMediaRef = MLE_NO_MEDIA;
    }
}

void
Mle2dImgRole::display(int state)
{
    displayState = state;
}

void
Mle2dImgRole::draw(void *data)
{
    if ((displayState == TRUE) && (data != NULL))
    {
        // 'data' should be a reference to a Set's offscreen buffer.
        // Draw the the 2D Image into the Set's offscreen buffer.
        QPixmap *buffer = (QPixmap *) data;
        QPainter painter(buffer);
        int x = screenPosition[0];
        int y = screenPosition[1];
        int width = m_surface.width();
        int height = m_surface.height();
        painter.drawPixmap(x, y, width, height, m_surface);
    }
}
