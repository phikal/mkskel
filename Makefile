# (MKSKEL) Makefile

# This file is in the public domain, to the extent possible under law,
# published under the CC0 1.0 Universal license.

# For a full copy of the CC0 license see
# https://creativecommons.org/publicdomain/zero/1.0/legalcode

BIN       = mkskel
OBJ       = main.o process.o skel.o list.o

all: mkskel

$(OBJ): mkskel.h

mkskel: $(OBJ)
	$(CC) -o $(BIN) $^

clean:
	rm -rf $(BIN) $(OBJ)

install: all
	cp -r mkskel /usr/bin/
	chmod 755 /usr/bin/mkskel

.PHONY: all clean install
