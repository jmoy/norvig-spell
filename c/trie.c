#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "trie.h"
#include "util.h"

#define MAXWORD 100

Trie *mk_trie()
{
  Trie *p = nv_alloc(sizeof(Trie));
  memset(p,0,sizeof(Trie));
  return p;
}

void increment(Trie *tp,const unsigned char *s)
{
  for (;*s;s++){
    if (tp->child[*s]==NULL)
      tp->child[*s] = mk_trie();
    tp = tp->child[*s];
  }
  tp->count++;
}

long lookup(const Trie *tp,const unsigned char *s)
{
  for (;*s;s++){
    if (tp->child[*s]==NULL)
      return 0;
    tp = tp->child[*s];
  }
  return tp->count;
}

Trie *train(FILE *fp)
{
  int c='.';
  int i;
  unsigned char wrd[MAXWORD+1];
  Trie *tp=mk_trie();

  while (c!=EOF){
    while ((c=getc(fp))!=EOF && !isalpha(c))
      ;
    i=0;
    while (isalpha(c) && i<MAXWORD){
      wrd[i++] = tolower(c);
      c=getc(fp);
    }
    wrd[i]='\0';
    if(i==MAXWORD){
      fputs("Word length exceeded\n",stderr);
      exit(1);
    }
    if (i>0)
      increment(tp,wrd);
  }
  return tp;
}
