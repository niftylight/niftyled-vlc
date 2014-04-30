#############
# VLC video output plugin for niftyled
# configure.ac - v0.5


# --------------------------------
#    initialization
# --------------------------------

# package info
AC_INIT([niftyled-vlc], m4_esyscmd([./version --short]), [http://github.com/niftylight/niftyled-vlc/issues], [niftyled-vlc], [http://wiki.niftylight.de/AC_PACKAGE_NAME])
AC_COPYRIGHT([Copyright (C) 2006-2014 Daniel Hiepler])

# for cross compiling
AC_CANONICAL_SYSTEM

# autotools initialization
AC_LANG(C)
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([color-tests dist-bzip2 parallel-tests silent-rules -Wno-portability -Wall])



# --------------------------------
#    further info on the package
# --------------------------------

# infos
PACKAGE_DESCRIPTION="VLC video output plugin for niftyled LED hardware"
PACKAGE_DESCRIPTION_LONG="video output plugin to use the VideoLAN client to output pixeldata to LED hardware using niftyled"

AC_SUBST([PACKAGE_DESCRIPTION_LONG], [${PACKAGE_DESCRIPTION_LONG}], [long package description])
AC_SUBST([PACKAGE_DESCRIPTION], [${PACKAGE_DESCRIPTION}], [package description])
AC_DEFINE_UNQUOTED(PACKAGE_DESCRIPTION_LONG, "${PACKAGE_DESCRIPTION_LONG}", [long package description])
AC_DEFINE_UNQUOTED(PACKAGE_DESCRIPTION, "${PACKAGE_DESCRIPTION}", [package description])

AC_SUBST([PACKAGE_MAJOR_VERSION], m4_esyscmd([./version --major]), [major version])
AC_SUBST([PACKAGE_MINOR_VERSION], m4_esyscmd([./version --minor]), [minor version])
AC_SUBST([PACKAGE_MICRO_VERSION], m4_esyscmd([./version --micro]), [micro version])
AC_SUBST([PACKAGE_GIT_VERSION], m4_esyscmd([./version --git]), [git version])
AC_SUBST([DEBIAN_REVISION], m4_esyscmd([./version --debian]), [debian revision])


# -----------------------------------------------
#    libtool stuff
# -----------------------------------------------
LT_INIT
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL


# --------------------------------
#    checks for programs
# --------------------------------
dnl change this to 0.28 as soon as it's widely used. Then also delete m4/pkg.m4
PKG_PROG_PKG_CONFIG([0.25])
if test -z "${PKG_CONFIG}"; then
    AC_MSG_ERROR([*** pkg-config not found. See http://www.freedesktop.org/software/pkgconfig/ or check your distribution.])
fi


# --------------------------------
#    checks for libraries
# --------------------------------

# test for VLC
PKG_CHECK_MODULES(vlc_plugin, [vlc-plugin >= 1.1.0])
AC_SUBST(vlc_plugin_CFLAGS)
AC_SUBST(vlc_plugin_LIBS)

# test for libniftyled
PKG_CHECK_MODULES(niftyled, [niftyled], [], [AC_MSG_ERROR([You need libniftyled + development headers installed])])
AC_SUBST(niftyled_CFLAGS)
AC_SUBST(niftyled_LIBS)


# --------------------------------
#    checks for header files
# --------------------------------
AC_HEADER_STDC


# --------------------------------
#    checks for types
# --------------------------------

# --------------------------------
#    checks for structures
# --------------------------------

# --------------------------------
#    checks for compiler characteristics
# --------------------------------

# --------------------------------
#    checks for library functions
# --------------------------------

# --------------------------------
#    checks for system services
# --------------------------------

# --------------------------------
#   compile process
# --------------------------------
AC_PROG_CC_C99
AM_PROG_CC_C_O
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])


# --------------------------------
# Arguments for configure-script
# --------------------------------
AC_ARG_ENABLE(
        debug,
	AS_HELP_STRING([--enable-debug], [enable debugging, default: no]),
    	[case "${enableval}" in
    	     yes) debug=true ;;
    	      no)  debug=false ;;
    	       *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
    	esac],
    	[debug=false])

AM_CONDITIONAL(DEBUG, test x$debug = xtrue)


# --------------------------------
# Output
# --------------------------------
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES(
[
    Makefile
    modules/Makefile
    modules/video_output/Makefile
    modules/video_output/src/Makefile
    gentoo/media-plugins/niftyled-vlc/niftyled-vlc-9999.ebuild
    debian/changelog
    debian/control.tmpl
    debian/rules.tmpl
    debian/watch
])
AC_OUTPUT




echo -e "
Configure report:

\tPackage.....................:  ${PACKAGE_STRING}
\tDescription.................:  ${PACKAGE_DESCRIPTION}
\tRevision....................:  ${PACKAGE_GIT_VERSION}    
\tURL.........................:  ${PACKAGE_URL}
\tBugreports..................:  ${PACKAGE_BUGREPORT}

\tInstall prefix..............:  ${prefix}
\tC compiler..................:  ${CC}
\tSystem CFLAGS...............:  ${CFLAGS}
\tSystem CXXFLAGS.............:  ${CXXFLAGS}
\tSystem LDFLAGS..............:  ${LDFLAGS}
"

