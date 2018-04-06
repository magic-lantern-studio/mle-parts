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
# Makefile - for making the release configuration of the Magic Lantern
# Mle3dCameraActor DLL.
#
# $Id$
#
#*************************************************************************

#
# Set which configuration this is...
#
CONFIGURATION = DllRelease

ISM_DEPTH = ../..
LEAF_DEPTH = ..

#
# Include local definitions.
#
include localdefs

#
# The actual work of building is in a directory above, in a file
# called "leafdefs".
#
include $(LEAF_DEPTH)/leafdefs

# Turn off STRICT compilation.
WIN_STRICT =

install: $(TARGETS)
	$(INSTALL) -d $(MLE_ROOT)/lib/rehearsal
	$(INSTALL) Mle3dCameraActor.lib $(MLE_ROOT)/lib/rehearsal
	$(INSTALL) -d $(MLE_ROOT)/bin/rehearsal
	$(INSTALL) Mle3dCameraActor.dll $(MLE_ROOT)/bin/rehearsal

export: $(OBJECTS)
	lib.exe -def -out:"Mle3dCameraActor.lib" $(OBJECTS)
	$(INSTALL) -d $(MLE_ROOT)/lib/rehearsal
	$(INSTALL) Mle3dCameraActor.lib $(MLE_ROOT)/lib/rehearsal

collect:
	$(IBCOLLECT) -log ../../../build/collect.raw \
                     -srcDir . \
                     -destDir "lib/rehearsal" \
                     -tags LIB \
                     $(TARGETS)
