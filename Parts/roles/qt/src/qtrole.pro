#-------------------------------------------------
#
# Project created by QtCreator 2017-06-12T13:47:12
#
#-------------------------------------------------

QT       += widgets opengl

TARGET = qtrole
TEMPLATE = lib

DEFINES += QTSET_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../include $$PWD/../../common/include /opt/MagicLantern/include /usr/local/include

DEFINES += MLE_DIGITAL_PLAYPRINT MLE_NOT_DLL

SOURCES += qtrole.cpp \
           2drole.cpp \
           2dimgrole.cpp

HEADERS += $$PWD/../include/qtrole.h\
           $$PWD/../include/qtrole_global.h \
           $$PWD/../include/2dimgrole.h \
           $$PWD/../../common/include/mle/2drole.h

unix {
    target.path = /opt/MagicLantern/lib/mle/qt
    headers.path = /opt/MagicLantern/include/mle/qt
    headers.files = $$HEADERS
    INSTALLS += target headers
}
