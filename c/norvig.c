#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "util.h"
#include "trie.h"
#include "editor.h"

#define MAXWORD 100


struct maxstate
{
  Trie *tp;
  long max;
  char *maxs;
};

typedef struct maxstate MaxState;
void update(MaxState *ms,char *t)
{
  long val = lookup(ms->tp,t);
  if (val>ms->max){
    if (ms->maxs!=NULL)
      free(ms->maxs);
    ms->max = val;
    ms->maxs = nv_dup(t);
  }
}

void updater(void *p,char *s,size_t len){
  update(p,s);
}


char *correct(Trie *tp,char *word,size_t len)
{
  MaxState ms;
  if (lookup(tp,word))
    return word;
  
  ms.tp = tp;
  ms.max = 0;
  ms.maxs = NULL;

  edits1(word,len,updater,&ms);
  if (ms.max)
    return ms.maxs;
  edits2(word,len,updater,&ms);
  if (ms.max)
    return ms.maxs;
  return word;
}

int main(int argc,char*argv[])
{
  if (argc!=2){
    fputs("Usage: norvig [training dataset]",stderr);
    return 2;
  }
  FILE *fp= fopen(argv[1],"r");
  if (fp==NULL){
    perror(argv[1]);
    return 1;
  }
  Trie *mod=train(fp);
  fclose(fp);
  
  char word[MAXWORD+1];
  while (!feof(stdin)){
    int c;
    int len=0;
    while ((c=getchar())!=EOF && c!='\n'){
      if (len==MAXWORD){
        word[len]='\0';
        fprintf(stderr,"Word too long: %s\n",word);
        exit(1);
      }
      word[len++] = tolower(c);
    }
    word[len]='\0';
    if (!len)
      continue;
    for (size_t n=0;n<len;n++)
      word[n] = tolower(word[n]);
    char *p = correct(mod,word,len);
    printf("%s, %s\n",word,p);
    if (p!=word)
      free(p);
  }
}
