QT += core gui

TARGET = unittest
TEMPLATE = app

CONFIG(debug, debug|release) {
# debug build
unix:!macx: LIBS += \
    -L$$PWD/../../build-mleqtrole-Desktop-Debug/src -lqtrole \
    -L/opt/MagicLantern/lib/mle/qt -lqtmref \
    -L/opt/MagicLantern/lib/mle/runtime -lmlert -lmlloaders \
    -L/opt/MagicLantern/lib -lplayprint -lmlutil -lmlmath -lpng

INCLUDEPATH += $$PWD/../include $$PWD/../../common/include /opt/MagicLantern/include /usr/local/include
DEPENDPATH += $$PWD/../../build-mleqtrole-Desktop-Debug/src
} else {
# release build
unix:!macx: LIBS += \
    -L$$PWD/../../build-mleqtrole-Desktop-Release/src -lqtrole \
    -L/opt/MagicLantern/lib/mle/qt -lqtmref \
    -L/opt/MagicLantern/lib/mle/runtime -lmlert -lmlloaders \
    -L/opt/MagicLantern/lib -lplayprint -lmlutil -lmlmath -lpng

INCLUDEPATH += $$PWD/../include $$PWD/../../common/include /opt/MagicLantern/include /usr/local/include
DEPENDPATH += $$PWD/../../build-mleqtrole-Desktop-Release/src
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
