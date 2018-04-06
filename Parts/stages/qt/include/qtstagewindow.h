/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file qtstagewindow.h
 * @ingroup MleParts
 *
 * This file defines the Qt Stage Window.
 *
 * @author Mark S. Millard
 * @date May 24, 2017
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

#ifndef __MLE_QTSTAGEWINDOW_H_
#define __MLE_QTSTAGEWINDOW_H_

// Include Qt header files.
#include <QObject>
#include <QMainWindow>

// Include Magic Lantern Qt Stage header files.
#include "qtstage_global.h"

class QEvent;
class QtStage;

class StageWindowWidget : public QWidget
{
    Q_OBJECT

  public:
    StageWindowWidget();

    QSize sizeHint() const override;

    void paintEvent(QPaintEvent *event) override;

  private:
    QBrush m_background;
    int m_width;
    int m_height;
};


class QTSTAGESHARED_EXPORT QtStageWindow : public QMainWindow
{
    Q_OBJECT

  public:
    /**
     * The default constructor.
     */
    QtStageWindow(QtStage *stage);

    /**
     * The desctructor.
     */
    virtual ~QtStageWindow(void);

    /**
     * Initialize the Stage window.
     */
    void init(void);

  public slots:
    void showEvent(QShowEvent *event);
    void closeEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

  protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;

    // The central widget for the Stage.
    StageWindowWidget *m_centralWidget;
    QtStage *m_stage;

  private:
    // Hide the default constructor.
    QtStageWindow() {};
};

#endif /* __MLE_QTSTAGEWINDOW_H_ */
