# -*- Makefile -*-
#
#  Copyright (C) 2000-2007  Wizzer Works
#
#  Wizzer Works makes available all content in this file ("Content").
#  Unless otherwise indicated below, the Content is provided to you
#  under the terms and conditions of the Common Public License Version 1.0
#  ("CPL"). A copy of the CPL is available at
#
#      http://opensource.org/licenses/cpl1.0.php
#
#  For purposes of the CPL, "Program" will mean the Content.
#
#  For information concerning this Makefile, contact Mark S. Millard,
#  of Wizzer Works at msm@wizzerworks.com.
#
#*************************************************************************
#
# Makefile for installing header files.
#
# $Id$
#
###########################################################################

ISM_DEPTH = ..
include $(ISM_DEPTH)/ismdefs

VPATH = $(ISM_DEPTH)/../common/include/mle

HEADERS = \
	3danimrp.h \
	3dcamarp.h \
	3dcamclp.h \
	3dcamfvp.h \
	3dcamtp.h \
	3dcharrp.h \
	3dcomapp.h \
	3derotp.h \
	3dgeomrp.h \
	3dgeorwp.h \
	3dltap.h \
	3dltdp.h \
	3dltip.h \
	3dltsp.h \
	3dlttp.h \
	3dmatlrp.h \
	3dmodelp.h \
	3dnodetp.h \
	3dnscalp.h \
	3dpickp.h \
	3dqrotp.h \
	3drendp.h \
	3drgeomp.h \
	3drstylp.h \
	3dscenep.h \
	3dtexmpp.h \
	3dtextrp.h \
	3dtranfp.h \
	3dtranlp.h \
	3dtranrp.h \
	3duscalp.h \
	myprop1p.h \
	myprop2p.h \
	myprop3p.h \
	$(NULL)

install: $(HEADERS)
	$(INSTALL) -d $(MLE_ROOT)/include/mle
	$(INSTALL) $^ $(MLE_ROOT)/include/mle

include $(ISMRULES)

collect:
	$(IBCOLLECT) -baseDir $(ISM_DEPTH)/.. \
                     -log ../build/collect.raw \
                     -srcDir $(ISM_DEPTH)/../common/include/mle \
                     -destDir "include/mle" \
                     -tags "INCLUDE" \
                     $(HEADERS)
