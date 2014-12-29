#ifndef __JMOY_TRIE__
#define __JMOY_TRIE__

/* A simple trie implementation to hold frequency counts*/
struct trie {
  long count;
  struct trie *child[256];
};

typedef struct trie Trie;

/* Allocate a trie structure and zero it
   TODO: a deallocator
*/
Trie *mk_trie();

/* Increment the count of 's', or insert 1
   if it does not alreay exist.
*/
void increment(Trie *tp,const char *s);

/* Lookup 's' in the trie and return its count.
   Return 0 if 's' is not in trie.
*/
long lookup(const Trie *tp,const char *s);

/* Return a trie holding the frequence of words in 
   'fp'. A word is a maximal string of alphabetic
   characters (as per isalpha).
*/
Trie *train(FILE *fp);

#endif
