dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(ivset-runtime, 1.0)
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
                Iv2dSet/Makefile
                Iv3dSet/Makefile
                include/Makefile)
AC_OUTPUT