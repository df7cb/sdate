dnl Process this file with autoconf to produce a configure script.
AC_INIT([sdate],[0.1],[cb@df7cb.de],[sdate])
AC_PREREQ(2.59)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_CONFIG_HEADERS([config.h])
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL
AC_PROG_CC

dnl Checks for programs.

dnl Checks for libraries.
dnl Replace `main' with a function in -ldl:
AC_CHECK_LIB(dl, dlopen)

dnl Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h unistd.h features.h sys/feature_tests.h pthread.h stdint.h inttypes.h grp.h endian.h sys/sysmacros.h sys/socket.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_CHECK_TYPE(mode_t, int)
AC_CHECK_TYPE(off_t, long)
AC_CHECK_TYPE(size_t, unsigned)

dnl This should really be done intelligently.

case $target_cpu:$target_os in
	(alpha*:linux*)
	(ia64*:linux*)
      		libcpath="/lib/libc.so.6.1"
		;;
	(*:linux*)
	        libcpath="/lib/libc.so.6"
		;;
	(*:k*bsd*-gnu)
	        libcpath="/lib/libc.so.0.1"
		;;
	(*:freebsd*)
		libcpath="/usr/lib/libc.so.4"
		;;
	(*:netbsd*)
		libcpath="/usr/lib/libc.so.12"
		;;
	(*:openbsd*)
		libcpath="/usr/lib/libc.so.25.2"
		;;
	(*:hpux*)
		libcpath="/usr/lib/hpux32/libc.so.1"
		;;
	(*:osf*)
		libcpath="/shlib/libc.so"
		;;
	(*:solaris*)
		libcpath="/lib/libc.so.1"
		;;
	(*)
		AC_MSG_WARN([don't know where libc is for $target_os on
			     $target_cpu, setting to /lib/libc.so])
		libcpath="/lib/libc.so"
		;;
esac

AC_DEFINE_UNQUOTED([LIBCPATH], "$libcpath", [path to libc shared object])

AC_CONFIG_FILES([Makefile scripts/Makefile doc/Makefile])
AC_OUTPUT

dnl Local variables:
dnl mode: m4
dnl End:
