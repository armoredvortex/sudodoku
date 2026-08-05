PREFIX ?= /usr/local

.PHONY: install uninstall

install:
	install -Dm755 bin/Release/sudodoku \
	    $(DESTDIR)$(PREFIX)/bin/sudodoku
	install -Dm644 resources/JetBrainsMono-Bold.ttf \
	    $(DESTDIR)$(PREFIX)/share/sudodoku/JetBrainsMono-Bold.ttf
	install -Dm644 resources/sudodoku.png \
	    $(DESTDIR)$(PREFIX)/share/pixmaps/sudodoku.png
	install -Dm644 resources/sudodoku.desktop \
	    $(DESTDIR)$(PREFIX)/share/applications/sudodoku.desktop
	install -Dm644 resources/puzzles/puzzles_easy.txt \
	    $(DESTDIR)$(PREFIX)/share/sudodoku/puzzles/puzzles_easy.txt
	install -Dm644 resources/puzzles/puzzles_medium.txt \
	    $(DESTDIR)$(PREFIX)/share/sudodoku/puzzles/puzzles_medium.txt
	install -Dm644 resources/puzzles/puzzles_hard.txt \
	    $(DESTDIR)$(PREFIX)/share/sudodoku/puzzles/puzzles_hard.txt

uninstall:
	rm -f  $(DESTDIR)$(PREFIX)/bin/sudodoku
	rm -rf $(DESTDIR)$(PREFIX)/share/sudodoku
	rm -f  $(DESTDIR)$(PREFIX)/share/pixmaps/sudodoku.png
	rm -f  $(DESTDIR)$(PREFIX)/share/applications/sudodoku.desktop
