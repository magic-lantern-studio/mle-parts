#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/runtime

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = Mle2dSet.la

#######################################
# Build information for each library

# Sources for MleIvSet
Mle2dSet_la_SOURCES = \
	$(top_srcdir)/../../iv2dset.cxx

# Linker options libTestProgram
Mle2dSet_la_LDFLAGS = -module -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
Mle2dSet_la_CPPFLAGS = \
    -DMLE_DIGITAL_PLAYPRINT \
    -DMLE_NOT_UTIL_DLL \
    -DMLE_NOT_MATH_DLL \
    -DMLE_NOT_RUNTIME_DLL \
    -DMLE_NOT_DWP_DLL \
    -DMLE_NOT_2DROLE_DLL \
    -DMLE_NOT_2DSET_DLL \
    -DMLE_NOT_IVSTAGE_DLL \
	-I$(top_srcdir)/include -I$(top_srcdir)/../.. \
	-I$(top_srcdir)/../../../../stages/inventor/include \
	-I$(top_srcdir)/../../../../roles/inventor/include \
	-I$(top_srcdir)/../../../../roles/common/include \
	-I$(MLE_ROOT)/include \
	-I/usr/local/include
