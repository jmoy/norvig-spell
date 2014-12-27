#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "util.h"
#include "hat-trie.h"
#include "editor.h"

#define MAXWORD 100

hattrie_t *train(FILE *fp)
{
  int c='.';
  int i;
  char wrd[MAXWORD+1];
  hattrie_t *tp=hattrie_create();

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
    if (i>0){
      value_t *p = hattrie_get(tp,wrd,i);
      *p+=1;
    }
  }
  return tp;
}


struct maxstate
{
  hattrie_t *tp;
  unsigned long max;
  char *maxs;
};

typedef struct maxstate MaxState;

/* 'update' allows us to use 'MaxState'
   as an accumulator, keeping track of the word
   with the maximum count as per the associated
   Trie.
*/
void update(MaxState *ms,char *t,size_t len)
{
  value_t *p = hattrie_tryget(ms->tp,t,len);
  unsigned long val = (p==NULL)?0:*p;
  if (val>ms->max){
    if (ms->maxs!=NULL)
      free(ms->maxs);
    ms->max = val;
    ms->maxs = nv_dup(t);
  }
}

/* To be passed as a callback to 'edits1'*/
void updater(void *p,char *s,size_t len){
  update(p,s,len);
}


/* Take a 'word' of length 'len' and return
   a corrected word. The return value might
   be 'word' itself or a newly allocated string.
   In the latter case the caller is responsible
   for freeing it.
*/
char *correct(hattrie_t *tp,char *word,size_t len)
{
  MaxState ms;
  if (hattrie_tryget(tp,word,len)!=NULL)
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
  hattrie_t *mod=train(fp);
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
    char *p = correct(mod,word,len);
    printf("%s, %s\n",word,p);
    if (p!=word)
      free(p);
  }
}
