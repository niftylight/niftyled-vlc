# niftyled VLC plugin Makefile


pluginsdir = $(shell pkg-config --variable=pluginsdir vlc-plugin )
voutdir = $(pluginsdir)/video_output


VOUT_PLUGIN = libvout_niftyled_plugin.so

VOUT_CFLAGS = \
	$(shell pkg-config --cflags vlc-plugin niftyled) \
	-D__PLUGIN__  -DMODULE_STRING=\"niftyled\" \
	-Wall -Wextra

VOUT_LDADD = \
	$(shell pkg-config --libs vlc-plugin niftyled)


all: $(VOUT_PLUGIN)
 
$(VOUT_PLUGIN): niftyled.o
	gcc -shared -std=gnu99 \
	    $(VOUT_CFLAGS) $(VOUT_LDADD) \
	    -Wl,-soname -Wl,$@ -o $@ $<
 
niftyled.o: modules/video_output/src/niftyled.c
	gcc -Wall -fPIC -c -std=gnu99 $(VOUT_CFLAGS) -o $@ $<
 
clean:
	rm -f niftyled.o $(VOUT_PLUGIN)


install: all
	mkdir -p $(DESTDIR)/$(voutdir)/
	install -m 0755 $(VOUT_PLUGIN) $(DESTDIR)/$(voutdir)/

install-strip: all
	mkdir -p $(DESTDIR)/$(voutdir)/
	install -s -m 0755 $(VOUT_PLUGIN) $(DESTDIR)/$(voutdir)/

uninstall:
	rm -f -- $(DESTDIR)/$(voutdir)/$(VOUT_PLUGIN)

.PHONY: all clean install uninstall
