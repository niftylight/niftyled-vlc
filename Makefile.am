#############
# VLC video output plugin for niftyled
# Makefile.am - v0.1

# AMFLAGS
ACLOCAL_AMFLAGS = -I m4


SUBDIRS = \
	modules

EXTRA_DIST = \
	gentoo \
	debian

# create .deb package
# needs dpkg-dev, debhelper
DEBTMPDIR=$(abs_top_builddir)/deb-tmp
DEBSRCDIR=$(DEBTMPDIR)/@PACKAGE_NAME@-@PACKAGE_VERSION@
TARGZFILE=$(DEBTMPDIR)/@PACKAGE_NAME@_@PACKAGE_VERSION@.orig.tar.gz

deb: dist-gzip
	@echo "Building Debian package..."; \
	rm --recursive --force "$(DEBTMPDIR)" ; \
	mkdir -p "$(DEBTMPDIR)" && \
	mv "$(top_builddir)/@PACKAGE_NAME@-@PACKAGE_VERSION@.tar.gz" "$(TARGZFILE)" && \
	tar --directory "$(DEBTMPDIR)/" --extract --gunzip --file "$(TARGZFILE)" && \
	cp -r "$(top_builddir)/dist/debian/"* "$(DEBSRCDIR)/debian" && \
	mv "$(DEBSRCDIR)/debian/rules.tmpl"                 "$(DEBSRCDIR)/debian/rules" && \
	mv "$(DEBSRCDIR)/debian/control.tmpl"               "$(DEBSRCDIR)/debian/control" && \
	cd "$(DEBSRCDIR)" && \
	dpkg-buildpackage -D

