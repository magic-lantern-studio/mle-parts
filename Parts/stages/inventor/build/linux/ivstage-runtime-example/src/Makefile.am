#######################################
# The list of executables we are building seperated by spaces
# the 'bin_' indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
#bin_PROGRAMS=exampleProgram

# Because ivstage-rehearsal-example is only a sample program we don't want it to be installed.
# The 'noinst_' prefix indicates that the following targets are not to be
# installed.
noinst_PROGRAMS=exampleProgram

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

ACLOCAL_AMFLAGS=-I ../m4

# Sources for the exampleProgram 
exampleProgram_SOURCES= ivstage-runtime-example.cpp rtestubs.cpp

# Libraries for exampleProgram
exampleProgram_LDADD = $(top_srcdir)/../ivstage-runtime/IvStage/.libs/MleIvStage.so \
	$(MLE_ROOT)/lib/mle/runtime/Mle2dSet.so \
	$(MLE_ROOT)/lib/mle/runtime/Mle3dSet.so \
	$(MLE_ROOT)/lib/mle/runtime/Mle2dRole.so \
	$(MLE_ROOT)/lib/mle/runtime/Mle3dRole.so \
	$(MLE_ROOT)/lib/mle/runtime/libivroles.so \
	$(MLE_ROOT)/lib/mle/runtime/Mle3dColorMapMediaRef.so \
	$(MLE_ROOT)/lib/mle/runtime/Mle3dModelMediaRef.so \
	$(MLE_ROOT)/lib/mle/runtime/Mle3dTextureMapMediaRef.so \
	-L$(MLE_ROOT)/lib/mle/runtime -lmlert -lmlloaders -lDPP -lplayprint \
	-L$(MLE_ROOT)/lib -lmlmath -lmlutil \
	-lCoin -lSoXt -lXmu

# Linker options for exampleProgram
#exampleProgram_LDFLAGS = -rpath `cd $(top_srcdir);pwd`/../ivstage-rehearsal/IvStage/.libs
exampleProgram_LDFLAGS = -Wl,-rpath,`cd $(top_srcdir);pwd`/../ivstage-runtime/IvStage/.libs

# Compiler options for exampleProgram
exampleProgram_CPPFLAGS = -DMLE_DIGITAL_PLAYPRINT -DMLE_NOT_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../../include \
	-I$(MLE_ROOT)/include
