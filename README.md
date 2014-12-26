Implementations of [Norvig's spelling corrector](http://norvig.com/spell-correct.html) in various languages.

Author: [Jyotirmoy Bhattacharya](http://www.jyotirmoy.net), `jyotirmoy@jyotirmoy.net`

## Languages
* [Python](https://github.com/jmoy/norvig-spell/blob/master/python2/norvig.py) (minor modification of Norvig's original program)
* [C++14](https://github.com/jmoy/norvig-spell/blob/master/cpp1y/norvig.cc)
* [Haskell](https://github.com/jmoy/norvig-spell/blob/master/haskell/norvig.hs)

## Benchmarking results

### bin/norvig_py

    /usr/bin/time bin/norvig_py data/train.txt </dev/null

0.70user 0.04system 0:00.76elapsed 97%CPU (0avgtext+0avgdata 75528maxresident)k
0inputs+0outputs (0major+19105minor)pagefaults 0swaps

    /usr/bin/time bin/norvig_py data/train.txt <data/test.txt >OUTFILE

33.63user 0.09system 0:33.97elapsed 99%CPU (0avgtext+0avgdata 75492maxresident)k
0inputs+16outputs (0major+19104minor)pagefaults 0swaps

    diff -U0 OUTFILE data/output.txt|wc -l

0

### bin/norvig_cc

    /usr/bin/time bin/norvig_cc data/train.txt </dev/null

0.79user 0.01system 0:00.81elapsed 98%CPU (0avgtext+0avgdata 18200maxresident)k
0inputs+0outputs (0major+6045minor)pagefaults 0swaps

    /usr/bin/time bin/norvig_cc data/train.txt <data/test.txt >OUTFILE

32.05user 2.72system 0:35.51elapsed 97%CPU (0avgtext+0avgdata 436504maxresident)k
0inputs+24outputs (0major+845824minor)pagefaults 0swaps

    diff -U0 OUTFILE data/output.txt|wc -l

5

### bin/norvig_hs

    /usr/bin/time bin/norvig_hs data/train.txt </dev/null

0.00user 0.00system 0:00.01elapsed 72%CPU (0avgtext+0avgdata 9972maxresident)k
0inputs+0outputs (0major+1781minor)pagefaults 0swaps

    /usr/bin/time bin/norvig_hs data/train.txt <data/test.txt >OUTFILE

28.47user 0.19system 0:28.79elapsed 99%CPU (0avgtext+0avgdata 95708maxresident)k
0inputs+16outputs (0major+23260minor)pagefaults 0swaps

    diff -U0 OUTFILE data/output.txt|wc -l

8
