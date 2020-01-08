QT += core gui

TARGET = unittest
TEMPLATE = app

unix:!macx: LIBS += \
    -L$$PWD/../build/build-mleqtmref-Desktop_Qt_5_12_6_GCC_64bit-Debug/src -lqtmref \
    -L/opt/MagicLantern/lib/mle/runtime -lmlert \
    -L/opt/MagicLantern/lib -lplayprint -lmlutil -lmlmath -lpng

INCLUDEPATH += $$PWD/../include /opt/MagicLantern/include /usr/local/include
DEPENDPATH += $$PWD/../build/build-mleqtmref-Desktop_Qt_5_12_6_GCC_64bit-Debug/src

DEFINES += MLE_DIGITAL_PLAYPRINT MLE_NOT_DLL

SOURCES += \
    main.cpp \
    rtestubs.cpp

unix {
    copyfiles.commands = cp $$PWD/wwlogo.png wwlogo.png
}

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
