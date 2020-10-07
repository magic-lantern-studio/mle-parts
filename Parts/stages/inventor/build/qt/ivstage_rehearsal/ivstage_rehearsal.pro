QT -= gui

TARGET = MleIvStage
TEMPLATE = lib
DEFINES += IVSTAGE_REHEARSAL_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wno-unused-but-set-parameter

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../../../include /opt/MagicLantern/include /usr/local/include

DEFINES += \
    MLE_NOT_DLL \
    MLE_DIGITAL_WORKPRINT \
    MLE_REHEARSAL \
    MLE_SOQT

SOURCES += \
    $$PWD/../../../src/antirot.cxx \
    $$PWD/../../../src/CubeDragger.cxx \
    $$PWD/../../../src/CubeManip.cxx \
    $$PWD/../../../src/ivstage.cxx \
    $$PWD/../../../src/MultiSearchAction.cxx \
    $$PWD/../../../src/Nudger.cxx \
    $$PWD/../../../src/Snapper.cxx \
    $$PWD/../../../src/Source.cxx \
    $$PWD/../../../src/SpotMarker.cxx \
    $$PWD/../../../src/Target.cxx

HEADERS += \
    $$PWD/../../../include/mle/MleIvStage.h \
    $$PWD/../../../include/mle/ivstage.h

# Default rules for deployment.
unix {
    target.path = /opt/MagicLantern/lib/mle/qt/rehearsal
    headers.path = /opt/MagicLantern/include/mle
    headers.files = $$HEADERS
    INSTALLS += target headers
}
!isEmpty(target.path): INSTALLS += target
