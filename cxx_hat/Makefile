COMMONDIR=../cxx-common
HATDIR+=../hat-trie/src
LIBDIR:=../hat-trie/src/.libs
LDFLAGS+=-L$(LIBDIR)
CXXFLAGS+=-Wall -O2 -std=c++1y -I $(HATDIR) -I$(COMMONDIR)

norvig: norvig.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS) -lhat-trie

norvig.o: norvig.cc $(COMMONDIR)/norvig.h

clean:
	rm -f norvig *.o
.PHONY: clean

