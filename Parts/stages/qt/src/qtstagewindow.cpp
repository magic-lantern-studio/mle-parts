/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file qtstagewindow.cpp
 * @ingroup MleParts
 *
 * This file implements the Qt Stage Window.
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

// Include Qt header files.
#include <qdebug.h>
#include <QColor>
#include <QStyleOption>

// Include Magic Lantern Qt Stage header files.
#include "qtstagewindow.h"
#include "qtstage.h"
#include "closeeventfilter.h"


StageWindowWidget::StageWindowWidget()
{
    m_width = 640; m_height = 480;
    resize(m_width, m_height);

    autoFillBackground();
    m_background.setColor(Qt::green);
}

QSize
StageWindowWidget::sizeHint() const
{
    return QSize(m_width, m_height);
}

void
StageWindowWidget::paintEvent(QPaintEvent *event)
{
    //qDebug() << "StageWindowWidget: received Paint event.";
}

QtStageWindow::QtStageWindow(QtStage *stage)
  : m_centralWidget(NULL)
{
    m_stage = stage;

    // Install an event filter to capture a Close event.
    CloseEventFilter *closeFilter = new CloseEventFilter(this);
    installEventFilter(closeFilter);
}

QtStageWindow::~QtStageWindow(void)
{
    // Don't need to delete m_centralWidget. Since QStageWindow is inherited
    // from QMainWindow,  QMainWindow takes ownership of the widget pointer
    // and deletes it at the appropriate time.
}

void
QtStageWindow::init()
{
    setWindowTitle(QObject::tr("Qt Stage"));

    m_centralWidget = new StageWindowWidget();
    m_centralWidget->setStyleSheet("background-color: black;");
    setCentralWidget(m_centralWidget);
}

void QtStageWindow::showEvent(QShowEvent *event)
{
    //qDebug() << "QtStageWindow: received Show event.";
    QtStage::processShowEvent(event);
}

void QtStageWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << "QtStageWindow: received Resize event.";
    QtStage::processResizeEvent(event);
}

void QtStageWindow::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << "QtStageWindow: received Mouse Press event.";
    QtStage::processMouseEvent(event);
}

void QtStageWindow::mouseReleaseEvent(QMouseEvent *event)
{
    //qDebug() << "QtStageWindow: received Mouse Release event.";
    QtStage::processMouseEvent(event);
}

void QtStageWindow::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "QtStageWindow: received Mouse Move event.";
    QtStage::processMouseEvent(event);
}

void QtStageWindow::closeEvent(QEvent *event)
{
    //qDebug() << "QtStageWindow: received Close event.";
    QtStage::exit(event);
}

void QtStageWindow::paintEvent(QPaintEvent *event)
{
    //qDebug() << "QtStageWindow: received Paint event.";

    // QPainter only works in context of the paintEvent.
    QPixmap *buffer = m_stage->getOffscreenBuffer();
    if (buffer)
    {
        // Draw the offscreen buffer into the Stage window.
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawPixmap(0, 0, *buffer);
    }

    // Notify registered event handlers that a Paint event occured.
    QtStage::processPaintEvent(event);
}
