Implementations of Norvig's spelling corrector
in C++14 and Haskell

Author: Jyotirmoy Bhattacharya
jyotirmoy@jyotirmoy.net
http://www.jyotirmoy.net

# Benchmarking results


## bin/norvig_py

### Timing: training only
0.68user 0.07system 0:01.64elapsed 46%CPU (0avgtext+0avgdata 75508maxresident)k
168inputs+0outputs (1major+19100minor)pagefaults 0swaps

### Timing: test data
33.63user 0.04system 0:33.76elapsed 99%CPU (0avgtext+0avgdata 75500maxresident)k
0inputs+16outputs (0major+19107minor)pagefaults 0swaps
### Output difference from Python version


## bin/norvig_cc

### Timing: training only
0.79user 0.02system 0:00.81elapsed 99%CPU (0avgtext+0avgdata 18256maxresident)k
0inputs+0outputs (0major+6047minor)pagefaults 0swaps

### Timing: test data
32.20user 2.70system 0:35.18elapsed 99%CPU (0avgtext+0avgdata 436428maxresident)k
0inputs+24outputs (0major+845825minor)pagefaults 0swaps
### Output difference from Python version
236c236
< choises, chooses
---
> choises, chaises


## bin/norvig_hs

### Timing: training only
0.00user 0.00system 0:00.01elapsed 72%CPU (0avgtext+0avgdata 9784maxresident)k
0inputs+0outputs (0major+1778minor)pagefaults 0swaps

### Timing: test data
31.22user 0.21system 0:32.39elapsed 97%CPU (0avgtext+0avgdata 95612maxresident)k
0inputs+16outputs (0major+23174minor)pagefaults 0swaps
### Output difference from Python version
236c236
< choises, chooses
---
> choises, chaises
341c341
< accesing, accusing
---
> accesing, acceding
