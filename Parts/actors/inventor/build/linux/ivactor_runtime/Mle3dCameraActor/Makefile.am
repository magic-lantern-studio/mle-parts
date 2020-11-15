#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/runtime

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = Mle3dCameraActor.la

#######################################
# Build information for each library

# Sources for Mle3dCameraActor
Mle3dCameraActor_la_SOURCES = \
	$(top_srcdir)/../../../../common/src/3dcama.cxx

# Linker options libTestProgram
Mle3dCameraActor_la_LDFLAGS = -module -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
Mle3dCameraActor_la_CPPFLAGS = -DMLE_DIGITAL_PLAYPRINT -DMLE_NOT_DLL \
	-I$(top_srcdir)/include -I$(top_srcdir)/../../../../common/include \
	-I$(MLE_ROOT)/include
