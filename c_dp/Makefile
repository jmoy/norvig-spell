CFLAGS+=-O2 -Wall -std=c99

ALL: norvig
.PHONY: ALL

norvig: util.o trie.o norvig.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

trie.o: trie.c trie.h util.h

util.o: util.c util.h

norvig.o: norvig.c util.h trie.h

clean:
	rm -f *.o norvig
.PHONY: clean
