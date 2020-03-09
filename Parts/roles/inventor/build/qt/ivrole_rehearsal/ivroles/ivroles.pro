QT -= gui

TARGET = ivroles
TEMPLATE = lib
DEFINES += IVROLE_REHEARSAL_LIBRARY

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

INCLUDEPATH += $$PWD/../../../include /opt/MagicLantern/include /usr/local/include

DEFINES += \
    MLE_NOT_DLL \
    MLE_DIGITAL_WORKPRINT \
    MLE_REHEARSAL

SOURCES += \
    $$PWD/../../../../src/MleIv3dCameraCarrier.cxx \
    $$PWD/../../../../src/MleIv3dColorMapCarrier.cxx \
    $$PWD/../../../../src/MleIv3dModelCarrier.cxx \
    $$PWD/../../../../src/MleIv3dTextureMapCarrier.cxx \
    $$PWD/../../../../src/MleIv3dTransformCarrier.cxx


HEADERS += \
    $$PWD/../../../../common/include/mle/2dimgd.h \
    $$PWD/../../../../common/include/mle/2drole.h \
    $$PWD/../../../../common/include/mle/3danimd.h \
    $$PWD/../../../../common/include/mle/3danimrc.h \
    $$PWD/../../../../common/include/mle/3dbboxc.h \
    $$PWD/../../../../common/include/mle/3dcamarc.h \
    $$PWD/../../../../common/include/mle/3dcamc.h \
    $$PWD/../../../../common/include/mle/3dcamclc.h \
    $$PWD/../../../../common/include/mle/3dcamfvc.h \
    $$PWD/../../../../common/include/mle/3dcamtc.h \
    $$PWD/../../../../common/include/mle/3dcharrc.h \
    $$PWD/../../../../common/include/mle/3dcomapc.h \
    $$PWD/../../../../common/include/mle/3dgeomrc.h \
    $$PWD/../../../../common/include/mle/3dgeorwc.h \
    $$PWD/../../../../common/include/mle/3dltac.h \
    $$PWD/../../../../common/include/mle/3dltdc.h \
    $$PWD/../../../../common/include/mle/3dltic.h \
    $$PWD/../../../../common/include/mle/3dltsc.h \
    $$PWD/../../../../common/include/mle/3dlttc.h \
    $$PWD/../../../../common/include/mle/3dmatlrc.h \
    $$PWD/../../../../common/include/mle/3dmodelc.h \
    $$PWD/../../../../common/include/mle/3dnodetc.h \
    $$PWD/../../../../common/include/mle/3dpickc.h \
    $$PWD/../../../../common/include/mle/3drendc.h \
    $$PWD/../../../../common/include/mle/3drgeomc.h \
    $$PWD/../../../../common/include/mle/3drstylc.h \
    $$PWD/../../../../common/include/mle/3dscenec.h \
    $$PWD/../../../../common/include/mle/3dtexmpc.h \
    $$PWD/../../../../common/include/mle/3dtextrc.h \
    $$PWD/../../../../common/include/mle/3dtranrc.h \
    $$PWD/../../../../common/include/mle/3dtransc.h \
    $$PWD/../../../../common/include/mle/Mle2dImageRole.h \
    $$PWD/../../../../common/include/mle/Mle2dRole.h \
    $$PWD/../../../../common/include/mle/Mle3dRole.h \
    $$PWD/../../../../common/include/mle/paletted.h \
    $$PWD/../../../include/mle/MleIv3dRole.h


WORKPRINTS += $$PWD/../../../../workprints/MleIv2dSet.wpf

# Default rules for deployment.
unix {
    target.path = /opt/MagicLantern/lib/mle/qt/rehearsal
    headers.path = /opt/MagicLantern/include/mle
    headers.files = $$HEADERS
    workprints.path = /opt/MagicLantern/include/workprints
    workprints.files = $$WORKPRINTS
    INSTALLS += target headers workprints
}
!isEmpty(target.path): INSTALLS += target
