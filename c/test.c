#include <stdio.h>
#include "trie.h"
#include "editor.h"

void check_expect(char *testname,int result)
{
  printf("%s: %s\n",
          testname,
          result?"SUCCEED":"FAIL");
}
          
void trie_tests()
{
  Trie *tp = mk_trie();
  increment(tp,"ket");
  increment(tp,"ketchup");
  increment(tp,"bat");
  increment(tp,"ketchup");
  check_expect("Trie 1",lookup(tp,"ket")==1);
  check_expect("Trie 2",lookup(tp,"ketchup")==2);
  check_expect("Trie 3",lookup(tp,"batman")==0);
  check_expect("Trie 4",lookup(tp,"rat")==0);
}

void printer(void *st,char *s,size_t len)
{
  printf("%s ",s);
}

void edit_tests()
{
  puts("Edit 1 'or': ");
  edits1("or",2,printer,NULL);
  putchar('\n');

  puts("Edit 2 'or: ");
  edits2("or",2,printer,NULL);
  putchar('\n');
}

int main()
{
  trie_tests();
  edit_tests();
}
  
