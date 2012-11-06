#############
# VLC video output plugin for niftyled
# Makefile.am - v0.1


# compile for debugging ?
if DEBUG    
    DEBUG_CFLAGS = -DDEBUG
endif


libdir = @libdir@/vlc/plugins/video_output





lib_LTLIBRARIES = libvout_niftyled_plugin.la

libvout_niftyled_plugin_la_SOURCES = \
    niftyled.c

libvout_niftyled_plugin_la_CFLAGS = \
    $(vlc_plugin_CFLAGS) \
    $(niftyled_CFLAGS) \
    $(DEBUG_CFLAGS) \
    -DMODULE_STRING=\"niftyled\" -Wall -Wextra

libvout_niftyled_plugin_la_LIBADD = \
    $(vlc_plugin_LIBS) $(niftyled_LIBS)

libvout_niftyled_plugin_la_LDFLAGS = \
    -avoid-version -module -export-symbol-regex ^vlc_entry

