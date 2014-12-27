INCLUDEDIR+=hat-trie/src
LIBDIR:=hat-trie/src/.libs
LDFLAGS+=-L$(LIBDIR)
CFLAGS+=-O2 -Wall -std=c99 -I $(INCLUDEDIR)
OBJECTS:=util.o norvig.o editor.o

ALL: norvig
.PHONY: ALL

hat-trie:
	make -C hat-trie
.PHONY: hat-trie

norvig: $(OBJECTS) hat-trie
	$(CC) -o $@ $(CFLAGS) $(OBJECTS) $(LDFLAGS) -lhat-trie

editor.o: editor.c editor.h util.h

util.o: util.c util.h

norvig.o: norvig.c util.h editor.h 

test: util.o trie.o test.o editor.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

test.o: test.c trie.h editor.h

clean:
	rm -f *.o norvig test
.PHONY: clean
