#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "editor.h"

static const char alphabet[]="abcdefghijklmnopqrstuvwxyz";
static const size_t NLETTERS = sizeof(alphabet)-1;

static void deletes(const char *s, size_t len,Callback f,void *st)
{
  char *t=nv_alloc(len);
  for (size_t n=0;n<len;++n){
    for (size_t i=0;i<n;i++)
      t[i]=s[i];
    for (size_t i=n+1;i<=len;i++)
      t[i-1]=s[i];
    f(st,t,len-1);
  }
  free(t);
}

static void inserts(const char *s, size_t len, Callback f,void *st)
{
  char *t=nv_alloc(len+2);
  for (size_t n=0;n<=len;++n){
    for (size_t i=0;i<n;i++)
      t[i]=s[i];
    for (size_t i=n;i<=len;i++)
      t[i+1]=s[i];
    for (size_t i=0;i<NLETTERS;i++){
      t[n] = alphabet[i];
      f(st,t,len+1);
    }
  }
  free(t);
}

static void replaces(const char *s, size_t len, Callback f,void *st)
{
  char *t=nv_alloc(len+1);
  strcpy(t,s);
  for (size_t n=0;n<len;++n){
    for (size_t i=0;i<NLETTERS;i++){
      t[n] = alphabet[i];
      f(st,t,len);
    }
    t[n] = s[n];
  }
  free(t);
}

static void transposes(const char *s, size_t len, Callback f,void *st)
{
  char *t=nv_alloc(len+1);
  strcpy(t,s);
  for (size_t n=0;n<len-1;++n){
    char c = t[n];
    t[n] = t[n+1];
    t[n+1] = c;
    f(st,t,len);
    c = t[n];
    t[n]=t[n+1];
    t[n+1]=c;
  }
  free(t);
}


void edits1(const char *word,size_t len,Callback f,void *st)
{
  deletes(word,len,f,st);
  replaces(word,len,f,st);
  transposes(word,len,f,st);
  inserts(word,len,f,st);
}

struct twostate
{
  Callback f;
  void *st;
};

static void editer2(void *p,char *s,size_t len){
  struct twostate *pp = p;
  edits1(s,len,pp->f,pp->st);
}

void edits2(const char *word,size_t len,Callback f,void *st){
  struct twostate s2 = {f,st};
  edits1(word,len,editer2,&s2);
}


