#############
# VLC video output plugin for niftyled
# configure.ac - v0.1


# --------------------------------
#    initialization
# --------------------------------
AC_INIT([niftyled-vlc], m4_esyscmd([./version --short]), [http://github.com/niftylight/niftyled-vlc/issues], [niftyled-vlc], [http://wiki.niftylight.de/AC_PACKAGE_NAME])
AC_COPYRIGHT([Copyright (C) 2006-2012 Daniel Hiepler])
AC_CANONICAL_SYSTEM
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([color-tests dist-bzip2 parallel-tests silent-rules -Wno-portability -Wall])



# --------------------------------
#    information on the package
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


# --------------------------------
#    checks for programs
# --------------------------------
AC_PROG_CC_C99
AM_PROG_CC_C_O
AM_PROG_LIBTOOL
PKG_PROG_PKG_CONFIG([0.14])
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
])
AC_OUTPUT




echo "
Configure report:

    Package.....................:  ${PACKAGE_STRING}
    Description.................:  ${PACKAGE_DESCRIPTION}
    Revision....................:  ${PACKAGE_GIT_VERSION}    
    URL.........................:  ${PACKAGE_URL}
    Bugreports..................:  ${PACKAGE_BUGREPORT}

    Install prefix..............:  ${prefix}
    C compiler..................:  ${CC}
    C++ compiler................:  ${CPP}
    System CFLAGS...............:  ${CFLAGS}
    System CXXFLAGS.............:  ${CXXFLAGS}
    System LDFLAGS..............:  ${LDFLAGS}
"

