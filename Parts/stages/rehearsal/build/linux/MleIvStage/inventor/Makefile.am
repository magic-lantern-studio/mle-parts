#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/inventor

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = MleIvStage.la

#######################################
# Build information for each library

# Sources for MleIvStage
MleIvStage_la_SOURCES = \
	../../../../../inventor/src/antirot.cxx \
	../../../../../inventor/src/CubeDragger.cxx \
	../../../../../inventor/src/CubeManip.cxx \
	../../../../../inventor/src/ivstage.cxx \
	../../../../../inventor/src/MultiSearchAction.cxx \
	../../../../../inventor/src/Nudger.cxx \
	../../../../../inventor/src/Snapper.cxx \
	../../../../../inventor/src/Source.cxx \
	../../../../../inventor/src/SpotMarker.cxx \
	../../../../../inventor/src/Target.cxx

# Linker options libTestProgram
MleIvStage_la_LDFLAGS = -module

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
MleIvStage_la_CPPFLAGS = -DMLE_DIGITAL_WORKPRINT -DMLE_NOT_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../../inventor/include \
	-I$(MLE_ROOT)/include \
	-I$(MLE_HOME)/Core/mlert/inventor/include
