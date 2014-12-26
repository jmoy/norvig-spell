Implementations of [Norvig's spelling corrector](http://norvig.com/spell-correct.html) in various languages.

Author: [Jyotirmoy Bhattacharya](http://www.jyotirmoy.net), `jyotirmoy@jyotirmoy.net`

## Languages
* [Python](https://github.com/jmoy/norvig-spell/blob/master/python2/norvig.py) (minor modification of Norvig's original program)
* [C++14](https://github.com/jmoy/norvig-spell/blob/master/cxx1y/norvig.cc)
* [Haskell](https://github.com/jmoy/norvig-spell/blob/master/haskell/norvig.hs)

## Benchmarking results

### bin/norvig_py

    /usr/bin/time bin/norvig_py data/train.txt </dev/null

0.69user 0.07system 0:00.77elapsed 98%CPU (0avgtext+0avgdata 75524maxresident)k
0inputs+0outputs (0major+19101minor)pagefaults 0swaps

    /usr/bin/time bin/norvig_py data/train.txt <data/test.txt >OUTFILE

33.09user 0.05system 0:33.36elapsed 99%CPU (0avgtext+0avgdata 75524maxresident)k
0inputs+16outputs (0major+19102minor)pagefaults 0swaps

    diff -U0 OUTFILE data/output.txt|wc -l

0

### bin/norvig_cc

    /usr/bin/time bin/norvig_cc data/train.txt </dev/null

0.79user 0.02system 0:00.82elapsed 98%CPU (0avgtext+0avgdata 18148maxresident)k
0inputs+0outputs (0major+6044minor)pagefaults 0swaps

    /usr/bin/time bin/norvig_cc data/train.txt <data/test.txt >OUTFILE

30.98user 2.81system 0:34.19elapsed 98%CPU (0avgtext+0avgdata 436520maxresident)k
0inputs+16outputs (0major+845824minor)pagefaults 0swaps

    diff -U0 OUTFILE data/output.txt|wc -l

5

### bin/norvig_hs

    /usr/bin/time bin/norvig_hs data/train.txt </dev/null

0.00user 0.00system 0:00.02elapsed 38%CPU (0avgtext+0avgdata 9916maxresident)k
0inputs+0outputs (0major+1780minor)pagefaults 0swaps

    /usr/bin/time bin/norvig_hs data/train.txt <data/test.txt >OUTFILE

28.46user 0.14system 0:28.80elapsed 99%CPU (0avgtext+0avgdata 95876maxresident)k
0inputs+16outputs (0major+23264minor)pagefaults 0swaps

    diff -U0 OUTFILE data/output.txt|wc -l

8
