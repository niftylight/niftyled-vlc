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
DEBSRCDIR=$(DEBTMPDIR)/@PACKAGE_NAME@-@PACKAGE_VERSION@-@DEBIAN_REVISION@

deb: dist-gzip
	@echo "Building Debian package..."; \
	rm --recursive --force "$(DEBTMPDIR)" ; \
	if ! [ -d debian ] ; then \
	    @echo "The files needed for building a Debian package are not" \
	        "included by default in the distribution. To build a package, check" \
	        "out the project from source control."; \
	    exit 1; \
	fi; \
	mkdir -p "$(DEBTMPDIR)" && \
	mv "$(top_builddir)/@PACKAGE@-@PACKAGE_VERSION@.tar.gz" "$(DEBTMPDIR)/@PACKAGE_NAME@_@PACKAGE_VERSION@.orig.tar.gz" && \
	tar --directory "$(DEBTMPDIR)/" --extract --gunzip --file "$(DEBTMPDIR)/@PACKAGE_NAME@_@PACKAGE_VERSION@.orig.tar.gz" && \
	cp -r "$(top_builddir)/debian/"* "$(DEBSRCDIR)/debian" && \
	cp "$(top_builddir)/debian/rules.tmpl" "$(DEBSRCDIR)/debian/rules" && \
	cp "$(top_builddir)/debian/control.tmpl" "$(DEBSRCDIR)/debian/control" && \
	cd "$(DEBSRCDIR)" && \
	dpkg-buildpackage -D -v"@PACKAGE_VERSION@-@DEBIAN_REVISION@"

