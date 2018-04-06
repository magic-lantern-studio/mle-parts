#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/rehearsal

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = Mle3dRole.la

#######################################
# Build information for each library

# Sources for MleIv3dRole
Mle3dRole_la_SOURCES = \
	../../../src/MleIv3dRole.cxx
	

# Linker options MleIv2dRole
Mle3dRole_la_LDFLAGS = -module

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
Mle3dRole_la_CPPFLAGS = -DMLE_REHEARSAL -DMLE_NOT_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../../common/include \
	-I$(top_srcdir)/../../../inventor/include \
	-I$(top_srcdir)/../../../../sets/inventor \
	-I$(top_srcdir)/../../../../mediaref/common/include \
	-I$(MLE_ROOT)/include

