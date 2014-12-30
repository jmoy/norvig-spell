#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "util.h"
#include "trie.h"

#define MAXWORD 100


struct maxstate
{
  long max;
  Trie *maxarg;
};

typedef struct maxstate MaxState;


/* To be passed as a callback to 'visit_neighbours'*/
void updater(void *state,Trie *tp){
  MaxState *ms = state;
  long count = get_value(tp);
  if (count > ms->max){
    ms->max = count;
    ms->maxarg = tp;
  }
}


/* Take a 'word' of length 'len' and return
   a corrected word. The return value might
   be 'word' itself or a newly allocated string.
   In the latter case the caller is responsible
   for freeing it.
*/
char *correct(Trie *tp,char *word)
{
  MaxState ms;
  if (lookup(tp,word))
    return word;
  ms.max = 0;
  ms.maxarg = NULL;

  visit_neighbours(tp,word,1,updater,&ms);
  if (ms.max)
    return get_key(ms.maxarg);

  visit_neighbours(tp,word,2,updater,&ms);
  if (ms.max)
    return get_key(ms.maxarg);

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
    char *p = correct(mod,word);
    printf("%s, %s\n",word,p);
    if (p!=word)
      free(p);
  }
}
