INCLUDEDIR+=../hat-trie/src
LIBDIR:=../hat-trie/src/.libs
LDFLAGS+=-L$(LIBDIR)
CXXFLAGS+=-O2 -std=c++1y -I $(INCLUDEDIR)

norvig: norvig.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS) -lhat-trie

clean:
	rm -f norvig *.o
.PHONY: clean

