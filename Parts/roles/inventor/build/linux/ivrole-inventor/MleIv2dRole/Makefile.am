#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/inventor

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = Mle2dRole.la

#######################################
# Build information for each library

# Sources for MleIv2dRole
Mle2dRole_la_SOURCES = \
	$(top_srcdir)/../../../src/MleIv2dRole.cxx
	

# Linker options MleIv2dRole
Mle2dRole_la_LDFLAGS = -module -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
Mle2dRole_la_CPPFLAGS = \
    -DMLE_DIGITAL_WORKPRINT \
    -DMLE_NOT_UTIL_DLL \
    -DMLE_NOT_MATH_DLL \
    -DMLE_NOT_RUNTIME_DLL \
    -DMLE_NOT_DWP_DLL \
    -DMLE_NOT_3DCOLORMAPMEDIAREF_DLL \
    -DMLE_NOT_3DMODELMEDIAREF_DLL \
    -DMLE_NOT_3DTEXTUREMAPMEDIAREF_DLL \
    -DMLE_NOT_2DROLE_DLL \
    -DMLE_NOT_3DROLE_DLL \
    -DMLE_NOT_CARRIER_DLL \
    -DMLE_NOT_PROPERTY_DLL \
    -DMLE_NOT_2DSET_DLL \
    -DMLE_NOT_3DSET_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../../../common/include \
	-I$(top_srcdir)/../../../../inventor/include \
	-I$(top_srcdir)/../../../../../sets/inventor \
	-I$(top_srcdir)/../../../../../mediaref/common/include \
	-I$(MLE_ROOT)/include
