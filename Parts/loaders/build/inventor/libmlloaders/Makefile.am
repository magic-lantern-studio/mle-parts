#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/inventor

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libmlloaders.la

#######################################
# Build information for each library

# Sources for libmlloaders
libmlloaders_la_SOURCES = \
    $(top_srcdir)/../../common/src/loadDwp.cxx \
    $(top_srcdir)/../../common/src/loadSet.cxx

# Linker options libTestProgram
libmlloaders_la_LDFLAGS = -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmlloaders_la_CPPFLAGS = \
	-DMLE_NOT_DLL \
	-DMLE_INTERNAL \
	-DMLE_DIGITAL_WORKPRINT \
    -I$(top_srcdir)/include \
    -I$(MLE_ROOT)/include \
    -std=c++17

    
libmlloaders_la_LIBDADD =

if HAVE_LOG4CXX
libmlloaders_la_CPPFLAGS += $(LOG4CXX_CFLAGS)
libmlloaders_la_LDFLAGS += -Wl,-rpath=/usr/local/lib
libmlloaders_la_LIBDADD += $(LOG4CXX_LIBS)
endif
    