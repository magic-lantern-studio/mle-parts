/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file qtpngcnvtr.cpp
 * @ingroup MleParts
 *
 * @author Mark S. Millard
 * @created July 5, 2017
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
// COPYRIGHT_END

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "qtpngcnvtr.h"

QtPngMrefConverter::QtPngMrefConverter()
{
    m_png = NULL;
}


QtPngMrefConverter::~QtPngMrefConverter()
{
    if (m_png) delete m_png;
}

MlBoolean
QtPngMrefConverter::getFile()
{
    MlBoolean status = TRUE;

    // Create a new QImage if necessary.
    if (! m_png)
        m_png = new QImage();

    status = m_png->load(m_reference, "PNG");

    return status;
}

MlBoolean
QtPngMrefConverter::releaseFile()
{
    if (m_png) delete m_png;
    m_png = NULL;
    return TRUE;
}

unsigned int
QtPngMrefConverter::getWidth()
{
    if (m_png)
        return m_png->width();
    else
        return 0;
}

unsigned int
QtPngMrefConverter::getHeight()
{
    if (m_png)
        return m_png->height();
    else
        return 0;
}

QImage *
QtPngMrefConverter::getData()
{
    return m_png;
}
