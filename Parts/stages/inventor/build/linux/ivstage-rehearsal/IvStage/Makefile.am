#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/rehearsal

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = MleIvStage.la

#######################################
# Build information for each library

# Sources for MleIvStage
MleIvStage_la_SOURCES = \
	../../../../src/antirot.cxx \
	../../../../src/CubeDragger.cxx \
	../../../../src/CubeManip.cxx \
	../../../../src/ivstage.cxx \
	../../../../src/MultiSearchAction.cxx \
	../../../../src/Nudger.cxx \
	../../../../src/Snapper.cxx \
	../../../../src/Source.cxx \
	../../../../src/SpotMarker.cxx \
	../../../../src/Target.cxx
	

# Linker options libTestProgram
MleIvStage_la_LDFLAGS = -module -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
MleIvStage_la_CPPFLAGS = -DMLE_REHEARSAL -DMLE_DIGITAL_WORKPRINT -DMLE_NOT_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../../include \
	-I$(MLE_ROOT)/include \
	-I$(MLE_HOME)/Core/mlert/rehearsal/include
