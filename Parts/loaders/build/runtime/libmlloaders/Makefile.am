#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/runtime

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libmlloaders.la

#######################################
# Build information for each library

# Sources for libmlloaders
libmlloaders_la_SOURCES = \
    $(top_srcdir)/../../common/src/loadSet.cxx

# Linker options libTestProgram
libmlloaders_la_LDFLAGS = -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmlloaders_la_CPPFLAGS = \
	-DMLE_NOT_DLL \
	-DMLE_INTERNAL \
	-DMLE_DIGITAL_PLAYPRINT \
    -I$(top_srcdir)/include \
	-I$(MLE_ROOT)/include \
	-std=c++17
