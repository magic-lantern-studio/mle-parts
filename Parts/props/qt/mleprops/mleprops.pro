TARGET = mleprops

SOURCES += \
    main.cpp

HEADERS += \
    $$PWD/../../common/include/mle/3danimrp.h \
    $$PWD/../../common/include/mle/3dcamarp.h \
    $$PWD/../../common/include/mle/3dcamclp.h \
    $$PWD/../../common/include/mle/3dcamfvp.h \
    $$PWD/../../common/include/mle/3dcamtp.h \
    $$PWD/../../common/include/mle/3dcharrp.h \
    $$PWD/../../common/include/mle/3dcomapp.h \
    $$PWD/../../common/include/mle/3derotp.h \
    $$PWD/../../common/include/mle/3dgeomrp.h \
    $$PWD/../../common/include/mle/3dgeorwp.h \
    $$PWD/../../common/include/mle/3dltap.h \
    $$PWD/../../common/include/mle/3dltdp.h \
    $$PWD/../../common/include/mle/3dltip.h \
    $$PWD/../../common/include/mle/3dltsp.h \
    $$PWD/../../common/include/mle/3dlttp.h \
    $$PWD/../../common/include/mle/3dmatlrp.h \
    $$PWD/../../common/include/mle/3dmodelp.h \
    $$PWD/../../common/include/mle/3dnodetp.h \
    $$PWD/../../common/include/mle/3dnscalp.h \
    $$PWD/../../common/include/mle/3dpickp.h \
    $$PWD/../../common/include/mle/3dqrotp.h \
    $$PWD/../../common/include/mle/3drendp.h \
    $$PWD/../../common/include/mle/3drgeomp.h \
    $$PWD/../../common/include/mle/3drstylp.h \
    $$PWD/../../common/include/mle/3dscenep.h \
    $$PWD/../../common/include/mle/3dtexmpp.h \
    $$PWD/../../common/include/mle/3dtextrp.h \
    $$PWD/../../common/include/mle/3dtranfp.h \
    $$PWD/../../common/include/mle/3dtranlp.h \
    $$PWD/../../common/include/mle/3dtranrp.h \
    $$PWD/../../common/include/mle/3duscalp.h \
    $$PWD/../../common/include/mle/myprop1p.h \
    $$PWD/../../common/include/mle/myprop2p.h \
    $$PWD/../../common/include/mle/myprop3p.h \
    $$PWD/../../common/include/mle/propdef.h

# Default rules for deployment.
unix {
    headers.path = /opt/MagicLantern/include/mle
    headers.files = $$HEADERS
    INSTALLS += headers
}
