#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/inventor

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = Mle3dColorMapMediaRef.la

#######################################
# Build information for each library

# Sources for MleMediaref
Mle3dColorMapMediaRef_la_SOURCES = \
	$(top_srcdir)/../../../../inventor/MleIv3dColorMapMediaRef.cxx

# Linker options libTestProgram
Mle3dColorMapMediaRef_la_LDFLAGS = -module -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
Mle3dColorMapMediaRef_la_CPPFLAGS = \
    -DMLE_DIGITAL_WORKPRINT \
    -DMLE_NOT_UTIL_DLL \
    -DMLE_NOT_DWP_DLL \
    -DMLE_NOT_RUNTIME_DLL \
    -DMLE_NOT_3DCOLORMAPMEDIAREF_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../../../common/include \
	-I$(top_srcdir)/../../../../inventor \
	-I$(MLE_ROOT)/include
