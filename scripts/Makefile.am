AUTOMAKE_OPTIONS=foreign
bin_SCRIPTS=sdate
EXTRA_DIST=sdate.in
CLEANFILES=sdate

do_subst = sed -e 's,[@]prefix[@],$(prefix),g' \
	   -e 's,[@]bindir[@],$(bindir),g' \
	   -e 's,[@]libdir[@],$(libdir),g' \
	   -e 's,[@]sdate_transformed[@],'`echo sdate | sed -e '$(transform)'`',g' \
	   -e 's,[@]VERSION[@],$(VERSION),g' 

sdate: $(srcdir)/sdate.in Makefile
	$(do_subst) < $(srcdir)/sdate.in > sdate
	chmod +x sdate
