#ifndef __JMOY_TRIE__
#define __JMOY_TRIE__

struct trie {
  long count;
  struct trie *child[256];
};

typedef struct trie Trie;

Trie *mk_trie();
void increment(Trie *tp,const char *s);
long lookup(const Trie *tp,const char *s);
Trie *train(FILE *fp);

#endif
