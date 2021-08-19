ACLOCAL_AMFLAGS=-I m4
AUTOMAKE_OPTIONS=foreign
SUBDIRS=scripts doc

lib_LTLIBRARIES=libsdate.la
libsdate_la_SOURCES=libsdate.c wrapdef.h  wrapstruct.h
libsdate_la_LDFLAGS=-version-info 0:1:0
libsdate_la_DEPENDENCIES=wrapdef.h wrapstruct.h

localtime_SOURCES=localtime.c
simple_SOURCES=simple.c
noinst_PROGRAMS=localtime simple

CLEANFILES=wrapdef.h wrapstruct.h wrapped.h wraptmpf.h

EXTRA_DIST=wrapawk wrapfunc.inp                        \
	   debian/rules debian/changelog debian/control  \
	   message.h \
	   DEBUG BUGS

wrapped.h wrapdef.h wrapstruct.h wraptmpf.h:wrapawk wrapfunc.inp
	awk -f $(srcdir)/wrapawk < $(srcdir)/wrapfunc.inp  

libsdate.lo:libsdate.c wrapdef.h wrapstruct.h wraptmpf.h 
