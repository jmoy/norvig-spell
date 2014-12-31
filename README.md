Implementations of [Norvig's spelling corrector](http://norvig.com/spell-correct.html) in various languages.

Author: [Jyotirmoy Bhattacharya](http://www.jyotirmoy.net), `jyotirmoy@jyotirmoy.net`

## Languages
* [Python](https://github.com/jmoy/norvig-spell/blob/master/python2/norvig.py) (minor modification of Norvig's original program)
* C++14 ([common header](https://github.com/jmoy/norvig-spell/blob/master/cxx-common/norvig.h))
  + [undordered_map](https://github.com/jmoy/norvig-spell/blob/master/cxx_umap/norvig.cc)
  + [hat-trie](https://github.com/jmoy/norvig-spell/blob/master/cxx_hat/norvig.cc)
* Haskell 
  + [HashMap](https://github.com/jmoy/norvig-spell/blob/master/haskell/norvig.hs) 
  + [Trie](https://github.com/jmoy/norvig-spell/blob/master/haskell-trie/norvig.hs) 
* C 
 + [dynamic programming](https://github.com/jmoy/norvig-spell/tree/master/c_dp) 
  
`hat-trie` is the fast trie [implementation](https://github.com/dcjones/hat-trie/) by dcjones.

## Building
The `hat-trie` library is included as a submodule. Doing 

    git submodule init
    git submodule update
    bash -c 'cd hat-trie && autoreconf -i && ./configure && make'

will build it. 

The Haskell programs depend on the `bytestring`, `bytestring-trie` and `unordered-containers` packages.

Once you have all the dependencies, running `make` at the top level will build all the programs and place them in `bin/`.

Each program can be run as

    norvig_xx [training data set]

where `[training data set]` is a plain text file from which the program will learn word frequencies. The programs expect to be given one word per line on standard input and print

    word, correction

pairs on standard output.

The folder `data/` has a training file `train.txt` and a test file `test.txt`. The first is exactly Norvig's `big.txt`. The second is Norvig's test set but with multiword strings removed.

## Performance

On Linux,running `make benchmark` creates a file `benchmarks/all.md` containing performance results.

The different implementations use different algorithms and code organization, so the benchmarks are not in any sense a comparison of the different languages.

This is what I get on my setup.

Version                 | Time (s) | Memory use<br/>(Max resident,M)
:-----------------------|---------:|----------------------------:
Python                  | 12.6     | 75.5
C++ (unordered_map)     |  6.0     |  5.6
C++ (hat-trie)          |  3.5     |  3.6
Haskell (HashMap)       | 11.5     | 81.3
Haskell (Trie)          | 22.3     |157.0
C (dynamic programming) |  0.3     |162.7


