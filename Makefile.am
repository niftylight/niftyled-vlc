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
DEBTMPDIR=deb-tmp
DEBSRCDIR=$(DEBTMPDIR)/$(PACKAGE_NAME)-$(VERSION)-$(PACKAGE_API_REVISION)
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
	mv "$(top_builddir)/$(PACKAGE)-$(VERSION).tar.gz" "$(DEBTMPDIR)/$(PACKAGE)_$(VERSION).orig.tar.gz" && \
	tar --directory "$(DEBTMPDIR)/" --extract --gunzip --file "$(DEBTMPDIR)/$(PACKAGE)_$(VERSION).orig.tar.gz" && \
	mv "$(DEBTMPDIR)/$(PACKAGE)-$(VERSION)" "$(DEBSRCDIR)" && \
	cp -r "$(top_builddir)/debian/"* "$(DEBSRCDIR)/debian" && \
	cp "$(top_builddir)/debian/control.tmpl" "$(DEBSRCDIR)/debian/control" && \
	cd "$(DEBSRCDIR)" && \
	dpkg-buildpackage -D -v"$(VERSION)-$(PACKAGE_API_REVISION)"

