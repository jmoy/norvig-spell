#!/usr/bin/env python2
# Modified from the version at: http://norvig.com/spell-correct.html

import re, collections, sys

def words(text): return re.findall('[a-z]+', text.lower()) 

def train(features):
    model = collections.defaultdict(lambda: 1)
    for f in features:
        model[f] += 1
    return model

NWORDS = train(words(open(sys.argv[1]).read()))

alphabet = 'abcdefghijklmnopqrstuvwxyz'

def edits1(word):
   splits     = [(word[:i], word[i:]) for i in range(len(word) + 1)]
   deletes    = [a + b[1:] for a, b in splits if b]
   transposes = [a + b[1] + b[0] + b[2:] for a, b in splits if len(b)>1]
   replaces   = [a + c + b[1:] for a, b in splits for c in alphabet if b]
   inserts    = [a + c + b     for a, b in splits for c in alphabet]
   return (deletes + transposes + replaces + inserts)

def known_edits2(word):
    return [e2 for e1 in edits1(word) for e2 in edits1(e1) if e2 in NWORDS]

def known(words): return set(word for word in words if word in NWORDS)

def correct(word):
    candidates = (known(set([word]))
                  or known(edits1(word))
                  or known_edits2(word)
                  or [word])
    return max(candidates, key=NWORDS.get)

if __name__=="__main__":
  for line in sys.stdin:
    line = line.strip()
    if not line:
      continue
    print("{}, {}".format(line,correct(line)))
