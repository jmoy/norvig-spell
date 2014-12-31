#include <string>
using namespace std;

#include "hat-trie.h"
#include "norvig.h"

class HatTrieFreq {
  hattrie_t *tp;
public:
  HatTrieFreq() {tp = hattrie_create();}
  void update(const string &word) {
    value_t *p = hattrie_get(tp,word.c_str(),word.length());
    *p += 1;
  }
  unsigned long lookup(const string &word) {
    value_t *p = hattrie_tryget(tp,word.c_str(),word.length());
    return (p==NULL)?0:*p;
  }
};

int main(int argc,char *argv[])
{
  Norvig<HatTrieFreq> norvig;
  return norvig.main(argc,argv);
}
