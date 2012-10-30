libdir = $(shell pkg-config --variable=libdir vlc-plugin )
vlclibdir = $(libdir)/vlc/plugins/video_output

all: libvout_niftyled_plugin.so
 
libvout_niftyled_plugin.so: niftyled.o
	gcc -shared -std=gnu99 `pkg-config --cflags --libs vlc-plugin niftyled`  -Wl,-soname -Wl,$@ -o $@ $<
 
niftyled.o: modules/video_output/src/niftyled.c
	gcc -Wall -fPIC -c -std=gnu99  `pkg-config --cflags --libs vlc-plugin niftyled` -D__PLUGIN__  -DMODULE_STRING=\"niftyled\" -o $@ $<
 
clean:
	rm -f niftyled.o libvout_niftyled_plugin.so

install: all
	mkdir -p $(DESTDIR)$(vlclibdir)/
	install -m 0755 libvout_niftyled_plugin.so $(DESTDIR)$(vlclibdir)/

install-strip: all
	mkdir -p $(DESTDIR)$(vlclibdir)/
	install -s -m 0755 libvout_niftyled_plugin.so $(DESTDIR)$(vlclibdir)/

uninstall:
	rm -f -- $(DESTDIR)$(vlclibdir)/libvout_niftyled_plugin.so

.PHONY: all clean install uninstall
