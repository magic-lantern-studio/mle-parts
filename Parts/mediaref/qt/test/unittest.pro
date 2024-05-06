QT += core gui

TARGET = unittest
TEMPLATE = app

CONFIG(debug, debug|release) {
# debug build
unix:!macx: LIBS += \
    -L$$PWD/../../build-mleqtmref-Desktop-Debug/src -lqtmref \
    -L/opt/MagicLantern/lib/mle/runtime -lmlert -lmlloaders \
    -L/opt/MagicLantern/lib -lplayprint -lmlutil -lmlmath -lpng

INCLUDEPATH += $$PWD/../include /opt/MagicLantern/include /usr/local/include
DEPENDPATH += $$PWD/../../build-mleqtmref-Desktop-Debug/src
} else {
# release build
unix:!macx: LIBS += \
    -L$$PWD/../../build-mleqtmref-Desktop-Release/src -lqtmref \
    -L/opt/MagicLantern/lib/mle/runtime -lmlert -lmlloaders \
    -L/opt/MagicLantern/lib -lplayprint -lmlutil -lmlmath -lpng

INCLUDEPATH += $$PWD/../include /opt/MagicLantern/include /usr/local/include
DEPENDPATH += $$PWD/../../build-mleqtmref-Desktop-Release/src
}

DEFINES += MLE_DIGITAL_PLAYPRINT MLE_NOT_DLL

SOURCES += \
    main.cpp \
    rtestubs.cpp

unix {
    copyfiles.commands = cp $$PWD/wwlogo.png wwlogo.png
}

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
