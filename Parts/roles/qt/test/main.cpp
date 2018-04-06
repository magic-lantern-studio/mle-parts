/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file main.cpp
 * @ingroup MleParts
 *
 * This file implements the Qt Media Reference unit test.
 *
 * @author Mark S. Millard
 * @date June 15, 2017
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
#include <QGuiApplication>
#include <QCommandLineParser>
#include <QDebug>

// Include Magic Lantern Qt header files.
#include "mle/qt/pngcnvtr.h"
#include "mle/qt/qtpngcnvtr.h"
#include "mle/qt/imagmref.h"
#include "mle/ppinput.h"
#include "mle/MleActor.h"
#include "2dimgrole.h"

int main(int argc, char *argv[])
{
    int status = -1;

    QGuiApplication app(argc, argv);

    // Intialize command line options parsing.
    QCommandLineParser parser;
    parser.setApplicationDescription("Unit test for Qt Role.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filename", QCoreApplication::translate("main", "PNG file name."));

    // Process the actual command line arguments given by the user.
    parser.process(app);

    // Get the PNG filename.
    const QStringList args = parser.positionalArguments();
    if (args.empty()) {
        // Show help string and exit with error status.
        parser.showHelp(status);
    }

    const QString filename = args.at(0);
    QByteArray ba = filename.toLatin1();
    char *c_filename = ba.data();

    /***** UNIT TEST FOR MleImageMediaRef ****/

    // Create a new Media Reference and initialize it.
    MleImageMediaRef *mref = new MleImageMediaRef();
    mref->init();

    // Read the Media Reference (the name of the PNG file) and load its contents into
    // a Qt pixmap. Normally this would be done while loading the Digital Playprint.
    QPixmap imgBuffer;
    status = mref->loadReference(c_filename);
    if (status)
        qDebug() << "MleImageMediaRef Unit Test: successfully loaded PNG file" << c_filename << ".";
    status = mref->read(&imgBuffer);
    if (status) {
        qDebug() << "MleImageMediaRef Unit Test: successfully read PNG data.";
        qDebug() << "MleImageMediaRef Unit Test: buffer width =" << imgBuffer.width();
        qDebug() << "MleImageMediaRef Unit Test: buffer height =" << imgBuffer.height();
        qDebug() << "MleImageMediaRef Unit Test: buffer depth =" << imgBuffer.depth();
        qDebug() << "MleImageMediaRef Unit Test: buffer alpha =" << imgBuffer.hasAlpha();
    }

    delete mref;

    /***** UNIT TEST FOR Mle2dImgRole ****/

    // Create a dummy actor.
    MleActor *actor = new MleActor();

    Mle2dImgRole *role = new Mle2dImgRole(actor);

    MlScalar min[2], max[2];
    role->getBounds(min, max);
    qDebug() << "MleImageMediaRef Unit Test: left =" << min[0];
    qDebug() << "MleImageMediaRef Unit Test: bottom =" << min[1];
    qDebug() << "MleImageMediaRef Unit Test: top =" << max[0];
    qDebug() << "MleImageMediaRef Unit Test: right =" << max[1];

    mref = new MleImageMediaRef();
    mref->init();
    status = mref->loadReference(c_filename);
    if (status)
        qDebug() << "MleImageMediaRef Unit Test: successfully loaded PNG file" << c_filename << ".";
    role->load(mref);

    delete role;
    delete mref;

    return status;
}
