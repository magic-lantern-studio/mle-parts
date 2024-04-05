#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/runtime

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = Mle3dModelMediaRef.la

#######################################
# Build information for each library

# Sources for MleMediaref
Mle3dModelMediaRef_la_SOURCES = \
	$(top_srcdir)/../../../../inventor/MleIv3dModelMediaRef.cxx

# Linker options libTestProgram
Mle3dModelMediaRef_la_LDFLAGS = -module -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
Mle3dModelMediaRef_la_CPPFLAGS = -DMLE_DIGITAL_PLAYPRINT -DMLE_NOT_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../../../common/include \
	-I$(top_srcdir)/../../../../inventor \
	-I$(MLE_ROOT)/include

