QT -= gui

TARGET = Mle3dSet
TEMPLATE = lib
DEFINES += IVSET_INVENTOR_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    $$PWD/../../../../../../roles/inventor/include \
    $$PWD/../../../../../../roles/common/include \
    $$PWD/../../../../../../stages/inventor/include \
    $$PWD/../../../.. \
    /opt/MagicLantern/include \
    /usr/local/include

DEFINES += \
    MLE_NOT_DLL \
    MLE_DIGITAL_WORKPRINT \
    MLE_QT \
    MLE_SOQT

SOURCES += \
    $$PWD/../../../../iv3dset.cxx

HEADERS += \
    $$PWD/../../../../mle/iv3dset.h \
    $$PWD/../../../../mle/Mle3dSet.h

WORKPRINTS += $$PWD/../../../../workprints/MleIv3dSet.wpf

macx {
    # Set the LFLAGS so that dynamic libraries behave like Linux DSOs.
    QMAKE_LFLAGS += -undefined suppress -flat_namespace
}

# Default rules for deployment.
unix {
    target.path = /opt/MagicLantern/lib/mle/qt/inventor
    headers.path = /opt/MagicLantern/include/mle
    headers.files = $$HEADERS
    workprints.path = /opt/MagicLantern/include/workprints
    workprints.files = $$WORKPRINTS
    INSTALLS += target headers workprints
}
#!isEmpty(target.path): INSTALLS += target
