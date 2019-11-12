dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(MleMediaref, 1.0)
AC_PROG_CC
AC_PROG_CXX

AC_PREFIX_DEFAULT(/opt/MagicLantern)

AC_CANONICAL_SYSTEM

AC_CONFIG_MACRO_DIR([m4])

dnl Initialize automake
AM_INIT_AUTOMAKE

dnl This allows us to specify individual linking flags for each target
AM_PROG_CC_C_O 

dnl Initialize Libtool
LT_INIT

AC_CONFIG_FILES(Makefile
                exampleProgram/Makefile
                Mle3dModelMediaref/Makefile
                Mle3dModelMediaref/rehearsal/Makefile
                Mle3dModelMediaref/runtime/Makefile
                Mle3dModelMediaref/inventor/Makefile
                Mle3dTextureMapMediaref/Makefile
                Mle3dTextureMapMediaref/rehearsal/Makefile
                Mle3dTextureMapMediaref/runtime/Makefile
                Mle3dTextureMapMediaref/inventor/Makefile
                Mle3dColorMapMediaref/Makefile
                Mle3dColorMapMediaref/rehearsal/Makefile
                Mle3dColorMapMediaref/runtime/Makefile
                Mle3dColorMapMediaref/inventor/Makefile
                Mle2dImageMediaref/Makefile
                Mle2dImageMediaref/rehearsal/Makefile
                Mle2dImageMediaref/runtime/Makefile
                Mle2dImageMediaref/inventor/Makefile
                include/Makefile)
AC_OUTPUT
