QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = unittest
TEMPLATE = app

unix:!macx: LIBS += \
    -L$$PWD/../build/build-mleqtset-Desktop_Qt_5_8_0_GCC_64bit-Debug/src -lqtset \
    -L/opt/MagicLantern/lib/mle/runtime -lmlert \
    -L/opt/MagicLantern/lib -lplayprint -lqtstage -lmlutil -lmlmath

#INCLUDEPATH += $$PWD/../build/build-mleqtset-Desktop_Qt_5_8_0_GCC_64bit-Debug
INCLUDEPATH += $$PWD/../include /opt/MagicLantern/include
DEPENDPATH += $$PWD/../build/build-mleqtset-Desktop_Qt_5_8_0_GCC_64bit-Debug/src

DEFINES += MLE_DIGITAL_PLAYPRINT MLE_NOT_DLL

SOURCES += \
    main.cpp \
    rtestubs.cpp
