EXECUTABLES:=bin/norvig_py bin/norvig_cc bin/norvig_hs
BENCHMARKS=$(EXECUTABLES:bin/%=benchmarks/%.md)

ALL: $(EXECUTABLES) README.md
.PHONY: ALL

bin/norvig_py: python2/norvig.py
	cp $< $@
bin/norvig_cc: cpp1y/norvig.cc
	cd cpp1y && make
	cp cpp1y/norvig $@
bin/norvig_hs: haskell/norvig.hs haskell/norvig.hs
	cd haskell \
		&& cabal configure --enable-optimization=2\
		&& cabal build
	cp haskell/dist/build/norvig/norvig $@

benchmarks/%.md: bin/% benchmark
	./benchmark $< > $@

README.md: Prelude.md $(BENCHMARKS)
	cat Prelude.md $(BENCHMARKS) > $@

#README.md: benchmark $(EXECUTABLES)
#	./benchmark $(EXECUTABLES) > $@

