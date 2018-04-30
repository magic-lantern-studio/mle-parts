#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/runtime

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = Mle3dSet.la

#######################################
# Build information for each library

# Sources for MleIvSet
Mle3dSet_la_SOURCES = \
	$(top_srcdir)/../iv3dset.cxx

# Linker options libTestProgram
Mle3dSet_la_LDFLAGS = -module

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
Mle3dSet_la_CPPFLAGS = -DMLE_NOT_DLL \
	-I$(top_srcdir)/include -I$(top_srcdir)/.. \
	-I$(top_srcdir)/../../../stages/inventor \
	-I$(top_srcdir)/../../../roles/inventor/include \
	-I$(top_srcdir)/../../../roles/common/include \
	-I$(MLE_ROOT)/include \
	-I/usr/local/include
