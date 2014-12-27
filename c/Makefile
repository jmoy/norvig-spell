CFLAGS+=-O2 -Wall -std=c99

ALL: norvig
.PHONY: ALL

norvig: util.o trie.o norvig.o editor.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

editor.o: editor.c editor.h util.h

trie.o: trie.c trie.h util.h

util.o: util.c util.h

norvig.o: norvig.c util.h trie.h editor.h

test: util.o trie.o test.o editor.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

test.o: test.c trie.h editor.h

clean:
	rm -f *.o norvig test
.PHONY: clean
