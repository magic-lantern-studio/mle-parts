/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file QtStageWindow.h
 * @ingroup MleParts
 *
 * This file defines a QtStageWindow that may be used as a top-level widget
 * for a Qt based stage.
 *
 * @author Mark S. Millard
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2020 Wizzer Works
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

#ifndef __QTSTAGEWINDOW_H_
#define __QTSTAGEWINDOW_H_

// Include Qt header files.
#include <QWidget>

// Include Magic Lantern header files.
#include "mle/MleIvStage.h"

/**
 * @brief The QtStageWindow class is used to provide a top-level window that can
 * be used as the parent QWidget for the MleIvStage.
 */
class REHEARSAL_API QtStageWindow : public QWidget
{
    Q_OBJECT

  public:

    /**
     * @brief Default constructor.
     */
    QtStageWindow(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());

    /**
     * @brief The destructor.
     */
    virtual ~QtStageWindow();

  protected:

    /**
     * @brief Override QtWidget close event handler.
     *
     * @param event The close event.
     */
    virtual void closeEvent(QCloseEvent *event);
};


#endif /* __QTSTAGEWINDOW_H_ */
