#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/runtime

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = MleEscapeActor.la

#######################################
# Build information for each library

# Sources for MleEscapeActor
MleEscapeActor_la_SOURCES = \
	$(top_srcdir)/../../../common/src/escapea.cxx

# Linker options libTestProgram
MleEscapeActor_la_LDFLAGS = -module

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
MleEscapeActor_la_CPPFLAGS = -DMLE_DIGITAL_PLAYPRINT -DMLE_NOT_DLL \
	-I$(top_srcdir)/include -I$(top_srcdir)/../../../common/include \
	-I$(MLE_ROOT)/include

