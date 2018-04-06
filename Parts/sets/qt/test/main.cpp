/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file main.cpp
 * @ingroup MleParts
 *
 * This file implements the Qt Set unit test.
 *
 * @author Mark S. Millard
 * @date Octobr 2, 2017
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
#include <QApplication>
#include <QScreen>
#include <QTimer>

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleDirector.h"

// Include QtStage header files.
#include "mle/qt/qtstage.h"
#include "qt2dset.h"

int mainloop(const QGuiApplication &app)
{
    int status = 0;

    while(! QtStage::g_doExit) {
        // Execute the scheduled phases in order of insertion.
        g_theTitle->m_theScheduler->goAll();

        // Process pending Qt events.
        app.processEvents();

        //qDebug() << "QtStage Unit Test: mainloop iteration.";
    }

    return status;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Initialize the title. Normally this would be in a title template.
    g_theTitle = new MleDirector();
    g_theTitle->m_theEventMgr = new MleEventDispatcher();
    g_theTitle->m_theScheduler = new MleScheduler();
    g_theTitle->m_theScheduler->init();

    // Create the stage and initialize it.
    QtStage *theStage = new QtStage();
    theStage->init();
    theStage->setName("QtSet Unit Test");
    theStage->show();

    // Quit after 10 seconds. For platforms that do not have windows that are closeable.
    if (QCoreApplication::arguments().contains(QStringLiteral("--timeout")))
        QTimer::singleShot(10000, qGuiApp, &QCoreApplication::quit);

    // Create a set and initialize it.
    Mle2dSet *aSet = new Mle2dSet();
    aSet->init();

    //const int exitValue = app.exec();
    const int exitValue = mainloop(app);

    // Clean up.
    delete aSet;
    delete theStage;

    return exitValue;
}
