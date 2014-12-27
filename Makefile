EXECUTABLES:=bin/norvig_py bin/norvig_cc bin/norvig_hs bin/norvig_hs_trie bin/norvig_c 
CABAL_MODULES:=haskell haskell-trie
MAKE_MODULES:=cxx1y c
ALL_MODULES:=$(CABAL_MODULES) $(MAKE_MODULES)
BENCHMARKS=$(EXECUTABLES:bin/%=benchmarks/%.md)
DATAFILES:=data/train.txt data/test.txt data/output.txt

ALL: $(ALL_MODULES) $(EXECUTABLES)

$(MAKE_MODULES):
	$(MAKE) -C $@

$(CABAL_MODULES):
	cd $@ &&\
	cabal configure --enable-optimization=2 &&\
	cabal build

$(OTHER_MODULES):

.PHONY: ALL $(ALL_MODULES)

bin/norvig_py: python2/norvig.py
	cp $< $@
bin/norvig_cc: cxx1y/norvig
	cp $< $@
bin/norvig_hs: haskell/dist/build/norvig/norvig
	cp $< $@
bin/norvig_hs_trie: haskell-trie/dist/build/norvig/norvig
	cp $< $@
bin/norvig_c: c/norvig
	cp $< $@


benchmark: benchmarks/all.md
.PHONY: benchmark

benchmarks/all.md: $(BENCHMARKS)
	cat $^ > $@
benchmarks/%.md: bin/% util/mk_benchmark $(DATAFILES)
	util/mk_benchmark $< > $@

clean:
	for dir in $(CABAL_MODULES); do\
		cd $$dir && cabal clean;\
	done
	for dir in $(MAKE_MODULES); do\
		$(MAKE) -C $$dir clean;\
	done
	rm -f $(EXECUTABLES) $(BENCHMARKS) benchmarks/all.md
.PHONY: clean

