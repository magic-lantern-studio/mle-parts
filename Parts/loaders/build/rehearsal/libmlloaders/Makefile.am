#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/rehearsal

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
libmlloaders_la_LDFLAGS =  -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmlloaders_la_CPPFLAGS = \
	-DMLE_NOT_DLL \
	-DMLE_INTERNAL \
	-DMLE_REHEARSAL \
	-DMLE_DIGITAL_WORKPRINT \
    -I$(top_srcdir)/include \
    -I$(MLE_HOME)/Core/util/common/include \
	-I$(MLE_HOME)/Core/util/linux/include \
	-I$(MLE_HOME)/DigitalWorkprint/lib/common/include \
	-I$(MLE_HOME)/DigitalWorkprint/lib/linux/include \
	-I$(MLE_HOME)/Core/mlert/common/include \
	-I$(MLE_HOME)/Core/mlert/common/src/foundation \
	-I$(MLE_HOME)/ATK/common/include \
	-I$(MLE_HOME)/ATK/linux/include

