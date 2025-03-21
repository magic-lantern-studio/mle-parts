#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/runtime

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = MleIvStage.la

#######################################
# Build information for each library

# Sources for MleIvStage
MleIvStage_la_SOURCES = \
	$(top_srcdir)/../../../src/antirot.cxx \
	$(top_srcdir)/../../../src/CubeDragger.cxx \
	$(top_srcdir)/../../../src/CubeManip.cxx \
	$(top_srcdir)/../../../src/ivstage.cxx \
	$(top_srcdir)/../../../src/MultiSearchAction.cxx \
	$(top_srcdir)/../../../src/Source.cxx \
	$(top_srcdir)/../../../src/SpotMarker.cxx \
	$(top_srcdir)/../../../src/Target.cxx
	

# Linker options libTestProgram
MleIvStage_la_LDFLAGS = -module -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
MleIvStage_la_CPPFLAGS = \
    -DMLE_DIGITAL_PLAYPRINT \
    -DMLE_NOT_UTIL_DLL \
    -DMLE_NOT_MATH_DLL \
    -DMLE_NOT_RUNTIME_DLL \
    -DMLE_NOT_DWP_DLL \
    -DMLE_NOT_PROPERTY_DLL \
    -DMLE_NOT_2DROLE_DLL \
    -DMLE_NOT_3DROLE_DLL \
    -DMLE_NOT_2DSET_DLL \
    -DMLE_NOT_3DSET_DLL \
    -DMLE_NOT_IVSTAGE_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../../include \
	-I$(MLE_ROOT)/include \
	-I$(MLE_HOME)/Core/mlert/linux/include
