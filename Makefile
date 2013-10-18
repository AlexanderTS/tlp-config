CFLAGS=-std=gnu99
LDFLAGS=-export-dynamic
EXECUTABLE=tlp-config-gui

SOURCES=./src/tlp-config.c ./src/io.c ./src/handlers.c
BUILD_DEPS=./src/tlp-config.h
RUN_DEPS=./src/main_window.glade ./src/warning_window.glade tlp.default
RUN_SCRIPT=./src/tlp-config-pkexec
PKGCONFIG=gtk+-3.0
PKGFLAGS=--cflags --libs

ICON=./desktop/tlp-config.png
ICONS=$(ICON).16 $(ICON).32 $(ICON).48 $(ICON).64 $(ICON).72 $(ICON).96 $(ICON).128
SHORTCUT=./desktop/tlp-config.desktop
POLICY=tlp-config.policy

EXEC_FOLDER=$(DESTDIR)/usr/bin/
CONFIG_FOLDER=$(DESTDIR)/etc/tlp-config/
POLICY_FOLDER=$(DESTDIR)/usr/share/polkit-1/actions/
ICON_FOLDER=$(DESTDIR)/usr/share/icons/hicolor/
SHORTCUT_FOLDER=$(DESTDIR)/usr/share/applications/

all: $(SOURCES) $(BUILD_DEPS)
	gcc $(CFLAGS) -o $(EXECUTABLE) $(SOURCES) $(LDFLAGS) `pkg-config $(PKGFLAGS) $(PKGCONFIG)`
		
install: $(EXECUTABLE) $(RUN_SCRIPT) $(RUN_DEPS) $(POLICY) $(SHORTCUT) $(ICONS)
	mkdir -p $(EXEC_FOLDER)
	mkdir -p $(CONFIG_FOLDER)
	mkdir -p $(POLICY_FOLDER)
	mkdir -p $(ICON_FOLDER)/128x128/apps
	mkdir -p $(ICON_FOLDER)/96x96/apps
	mkdir -p $(ICON_FOLDER)/72x72/apps
	mkdir -p $(ICON_FOLDER)/64x64/apps
	mkdir -p $(ICON_FOLDER)/48x48/apps
	mkdir -p $(ICON_FOLDER)/32x32/apps
	mkdir -p $(ICON_FOLDER)/16x16/apps
	mkdir -p $(SHORTCUT_FOLDER)

	cp $(EXECUTABLE) $(EXEC_FOLDER)
	cp $(RUN_SCRIPT) $(EXEC_FOLDER)/tlp-config

	chmod 755 $(EXEC_FOLDER)$(EXECUTABLE)
	chmod 755 $(EXEC_FOLDER)tlp-config
	cp $(RUN_DEPS) $(CONFIG_FOLDER)
	cp $(POLICY) $(POLICY_FOLDER)
	cp $(ICON).16 $(ICON_FOLDER)/16x16/apps/tlp-config.png
	cp $(ICON).32 $(ICON_FOLDER)/32x32/apps/tlp-config.png
	cp $(ICON).48 $(ICON_FOLDER)/48x48/apps/tlp-config.png
	cp $(ICON).64 $(ICON_FOLDER)/64x64/apps/tlp-config.png
	cp $(ICON).72 $(ICON_FOLDER)/72x72/apps/tlp-config.png
	cp $(ICON).96 $(ICON_FOLDER)/96x96/apps/tlp-config.png
	cp $(ICON).128 $(ICON_FOLDER)/128x128/apps/tlp-config.png
	cp $(SHORTCUT) $(SHORTCUT_FOLDER)

uninstall:
	rm -f $(EXEC_FOLDER)/tlp-config $(EXEC_FOLDER)$(EXECUTABLE) $(POLICY_FOLDER)$(POLICY) $(ICON_FOLDER)/16x16/apps/tlp-config.png $(ICON_FOLDER)/32x32/apps/tlp-config.png $(ICON_FOLDER)/48x48/apps/tlp-config.png $(ICON_FOLDER)/64x64/apps/tlp-config.png $(ICON_FOLDER)/72x72/apps/tlp-config.png $(ICON_FOLDER)/96x96/apps/tlp-config.png $(ICON_FOLDER)/128x128/apps/tlp-config.png$(SHORTCUT_FOLDER)$(SHORTCUT)
	rm -rf $(CONFIG_FOLDER)
	rm -f $(ICON_FOLDER)/16x16/apps/tlp-config.png
	rm -f $(ICON_FOLDER)/32x32/apps/tlp-config.png
	rm -f $(ICON_FOLDER)/48x48/apps/tlp-config.png
	rm -f $(ICON_FOLDER)/64x64/apps/tlp-config.png
	rm -f $(ICON_FOLDER)/72x72/apps/tlp-config.png
	rm -f $(ICON_FOLDER)/96x96/apps/tlp-config.png
	rm -f $(ICON_FOLDER)/128x128/apps/tlp-config.png
	rm -f $(SHORTCUT_FOLDER)$(SHORTCUT)
	rm -f $(POLICY_FOLDER)$(POLICY)

clean:
	rm -f $(EXECUTABLE)
