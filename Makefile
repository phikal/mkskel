# (MKSKEL) Makefile

# This file is in the public domain, to the extent possible under law,
# published under the CC0 1.0 Universal license.

# For a full copy of the CC0 license see
# https://creativecommons.org/publicdomain/zero/1.0/legalcode

BIN = mkskel
OBJ = main.o process.o skel.o list.o

all: mkskel

$(OBJ): mkskel.h

mkskel: $(OBJ)
	$(CC) -o $(BIN) $^

clean:
	rm -rf $(BIN) $(OBJ)

DESTDIR ?= /usr/local
MANDIR  ?= /man

install: all
	install -c -s -m 755 $(BIN) $(DESTDIR)$(PREFIX)/bin
	install -c -m 644 $(BIN).1 $(DESTDIR)$(MANDIR)/man1/

uninstall:
	rm -rf	$(DESTDIR)$(PREFIX)/bin/$(BIN) \
			$(DESTDIR)$(MANDIR)/man1/$(BIN).1

.PHONY: all clean install
