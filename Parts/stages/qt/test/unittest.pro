QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = unittest
TEMPLATE = app

CONFIG(debug, debug|release) {
# debug build
unix:!macx: LIBS += \
    -L$$PWD/../build/build-mleqtstage-Desktop_Qt_5_12_6_GCC_64bit-Debug/src -lqtstage \
    -L/opt/MagicLantern/lib/mle/runtime -lmlert \
    -L/opt/MagicLantern/lib -lplayprint -lmlutil -lmlmath

#INCLUDEPATH += $$PWD/../build/build-mleqtstage-Desktop_Qt_5_8_0_GCC_64bit-Debug
#INCLUDEPATH += $$PWD/../include /opt/MagicLantern/include /usr/local/include
INCLUDEPATH += $$PWD/../include /opt/MagicLantern/include
DEPENDPATH += $$PWD/../build/build-mleqtstage-Desktop_Qt_5_12_6_GCC_64bit-Debug/src
} else {
# release build
unix:!macx: LIBS += \
    -L$$PWD/../build/build-mleqtstage-Desktop_Qt_5_12_6_GCC_64bit-Release/src -lqtstage \
    -L/opt/MagicLantern/lib/mle/runtime -lmlert \
    -L/opt/MagicLantern/lib -lplayprint -lmlutil -lmlmath

#INCLUDEPATH += $$PWD/../build/build-mleqtstage-Desktop_Qt_5_8_0_GCC_64bit-Debug
#INCLUDEPATH += $$PWD/../include /opt/MagicLantern/include /usr/local/include
INCLUDEPATH += $$PWD/../include /opt/MagicLantern/include
DEPENDPATH += $$PWD/../build/build-mleqtstage-Desktop_Qt_5_12_6_GCC_64bit-Release/src
}

DEFINES += MLE_DIGITAL_PLAYPRINT MLE_NOT_DLL

SOURCES += \
    main.cpp \
    rtestubs.cpp
