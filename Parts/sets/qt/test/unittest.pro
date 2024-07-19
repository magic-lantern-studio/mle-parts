QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = unittest
TEMPLATE = app

CONFIG(debug, debug|release) {
# debug build
unix:!macx: LIBS += \
    -L$$PWD/../../build-mleqtset-Desktop-Debug/src -lqtset \
    -L/opt/MagicLantern/lib/mle/runtime -lmlert -lmlloaders \
    -L/opt/MagicLantern/lib/mle/qt -lqtstage \
    -L/opt/MagicLantern/lib -lplayprint -lmlutil -lmlmath

#INCLUDEPATH += $$PWD/../build/build-mleqtset-Desktop_Qt_5_12_6_GCC_64bit-Debug
INCLUDEPATH += $$PWD/../include /opt/MagicLantern/include
DEPENDPATH += $$PWD/../../build-mleqtset-Desktop-Debug/src
} else {
# release build
unix:!macx: LIBS += \
    -L$$PWD/../../build-mleqtset-Desktop-Release/src -lqtset \
    -L/opt/MagicLantern/lib/mle/runtime -lmlert -lmlloaders \
    -L/opt/MagicLantern/lib/mle/qt -lqtstage \
    -L/opt/MagicLantern/lib -lplayprint -lmlutil -lmlmath

#INCLUDEPATH += $$PWD/../build/build-mleqtset-Desktop_Qt_5_12_6_GCC_64bit-Debug
INCLUDEPATH += $$PWD/../include /opt/MagicLantern/include
DEPENDPATH += $$PWD/../../build-mleqtset-Desktop-Debug/src
}

DEFINES += MLE_DIGITAL_PLAYPRINT MLE_NOT_DLL

SOURCES += \
    main.cpp \
    rtestubs.cpp
