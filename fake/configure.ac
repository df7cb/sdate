dnl Process this file with autoconf to produce a configure script.
AC_INIT
AC_CONFIG_SRCDIR([libsdate.c])
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE(sdate, 0.1.0)
AM_MAINTAINER_MODE
AC_CONFIG_AUX_DIR(..)

dnl AM_CONFIG_HEADER(config.h)

AM_PROG_LIBTOOL
AC_PROG_CC
dnl Checks for programs.

dnl Checks for libraries.
dnl Replace `main' with a function in -ldl:
AC_CHECK_LIB(dl, dlopen)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
