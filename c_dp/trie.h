#ifndef __JMOY_TRIE__
#define __JMOY_TRIE__
#include <limits.h>

/* A simple trie implementation to hold frequency counts*/

#define JTRIE_NUM_CHILD (CHAR_MAX-CHAR_MIN+1)

struct trie {
  long count;
  long depth;
  char c;
  struct trie *child[JTRIE_NUM_CHILD];
  struct trie *prev;
};

typedef struct trie Trie;

typedef void (*callback_t)(void *state,Trie *tp);

/* Allocate a trie structure and zero it
   TODO: a deallocator
*/
Trie *mk_trie();

/* Increment the count of 's', or insert 1
   if it does not alreay exist.
*/
void increment(Trie *tp,const char *s);

/* Get the key corresponding to a trie node
   The key is stored in a malloced array
   which the caller must free*/
char *get_key(Trie *tp);

/* Visit all words in trie at most 'maxedit' edits
   away from 's'. At each of those words call the
   given callback 'f' with the address of the node 
   and state
*/
void visit_neighbours(Trie *tp,char *s,int maxedit,
                      callback_t f,void *state);


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
