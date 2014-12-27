EXECUTABLES:=bin/norvig_py bin/norvig_cc bin/norvig_hs bin/norvig_c
BENCHMARKS=$(EXECUTABLES:bin/%=benchmarks/%.md)
DATAFILES:=data/train.txt data/test.txt data/output.txt

ALL: $(EXECUTABLES)
.PHONY: ALL

bin/norvig_py: python2/norvig.py
	cp $< $@
bin/norvig_cc: 
	cd cxx1y && make
	cp cxx1y/norvig $@
bin/norvig_hs: haskell/norvig.hs haskell/norvig.hs
	cd haskell \
		&& cabal configure --enable-optimization=2\
		&& cabal build
	cp haskell/dist/build/norvig/norvig $@
bin/norvig_c:
	cd c && make
	cp c/norvig $@

benchmark: benchmarks/all.md
.PHONY: benchmark

benchmarks/all.md: $(BENCHMARKS)
	cat $^ > $@
benchmarks/%.md: bin/% util/mk_benchmark $(DATAFILES)
	util/mk_benchmark $< > $@

clean:
	cd haskell && cabal clean
	cd cxx1y && make clean
	rm -f $(EXECUTABLES) $(BENCHMARKS) benchmarks/all.md
.PHONY: clean

