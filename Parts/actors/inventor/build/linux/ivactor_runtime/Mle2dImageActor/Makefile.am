#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/runtime

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = Mle2dImageActor.la

#######################################
# Build information for each library

# Sources for MleActors
Mle2dImageActor_la_SOURCES = \
	$(top_srcdir)/../../../../common/src/2dimga.cxx

# Linker options libTestProgram
Mle2dImageActor_la_LDFLAGS = -module -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
Mle2dImageActor_la_CPPFLAGS = \
    -DMLE_DIGITAL_PLAYPRINT \
    -DMLE_NOT_UTIL_DLL \
    -DMLE_NOT_MATH_DLL \
    -DMLE_NOT_RUNTIME_DLL \
    -DMLE_NOT_DWP_DLL \
    -DMLE_NOT_2DROLE_DLL \
    -DMLE_NOT_2DIMAGEROLE_DLL \
    -DMLE_NOT_2DIMAGEACTOR_DLL \
	-I$(top_srcdir)/include -I$(top_srcdir)/../../../../common/include \
	-I$(MLE_ROOT)/include
