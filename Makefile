EXECUTABLES:=bin/norvig_py bin/norvig_cc bin/norvig_hs
BENCHMARKS=$(EXECUTABLES:bin/%=benchmarks/%.md)

ALL: $(EXECUTABLES) README.md
.PHONY: ALL

bin/norvig_py: python2/norvig.py
	cp $< $@
bin/norvig_cc: cxx1y/norvig.cc
	cd cxx1y && make
	cp cxx1y/norvig $@
bin/norvig_hs: haskell/norvig.hs haskell/norvig.hs
	cd haskell \
		&& cabal configure --enable-optimization=2\
		&& cabal build
	cp haskell/dist/build/norvig/norvig $@

benchmarks/%.md: bin/% benchmark
	./benchmark $< > $@

README.md: Prelude.md $(BENCHMARKS)
	cat Prelude.md $(BENCHMARKS) > $@

clean:
	cd haskell && cabal clean
	rm -f $(EXECUTABLES) $(BENCHMARKS) README.md
.PHONY: clean

