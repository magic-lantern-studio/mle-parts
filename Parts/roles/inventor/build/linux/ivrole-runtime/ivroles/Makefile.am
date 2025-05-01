#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/runtime

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libivroles.la

#######################################
# Build information for each library

# Sources for ivroles
libivroles_la_SOURCES = \
	$(top_srcdir)/../../../src/MleIv3dCameraCarrier.cxx \
	$(top_srcdir)/../../../src/MleIv3dColorMapCarrier.cxx \
	$(top_srcdir)/../../../src/MleIv3dModelCarrier.cxx \
	$(top_srcdir)/../../../src/MleIv3dTextureMapCarrier.cxx \
	$(top_srcdir)/../../../src/MleIv3dTransformCarrier.cxx
	

# Linker options libTestProgram
libivroles_la_LDFLAGS = -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libivroles_la_CPPFLAGS = \
    -DMLE_DIGITAL_PLAYPRINT \
    -DMLE_NOT_UTIL_DLL \
    -DMLE_NOT_MATH_DLL \
    -DMLE_NOT_RUNTIME_DLL \
    -DMLE_NOT_DWP_DLL \
    -DMLE_NOT_3DCOLORMAPMEDIAREF_DLL \
    -DMLE_NOT_3DMODELMEDIAREF_DLL \
    -DMLE_NOT_3DTEXTUREMAPMEDIAREF_DLL \
    -DMLE_NOT_3DMODELCARRIER_DLL \
    -DMLE_NOT_3DCOLORMAPCARRIER_DLL \
    -DMLE_NOT_3DTEXTUREMAPCARRIER_DLL \
    -DMLE_NOT_3DTRANSFORMCARRIER_DLL \
    -DMLE_NOT_3DCAMERACARRIER_DLL \
    -DMLE_NOT_2DROLE_DLL \
    -DMLE_NOT_3DROLE_DLL \
    -DMLE_NOT_PROPERTY_DLL \
    -DMLE_NOT_3DSET_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../../../common/include \
	-I$(top_srcdir)/../../../../inventor/include \
	-I$(top_srcdir)/../../../../../sets/inventor \
	-I$(top_srcdir)/../../../../../mediaref/common/include \
	-I$(MLE_ROOT)/include
