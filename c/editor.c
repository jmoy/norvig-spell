#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "editor.h"

static const unsigned char alphabet[]="abcdefghijklmnopqrstuvwxyz";
static const size_t NLETTERS = sizeof(alphabet)-1;

/*
deletes, inserts, replaces, trasposes, edits1 and edits2
all have the same prototype

void f(const unsigned char *s, size_t len, Callback cb, void *st)

where 's' is assumed to be a string of length 'len'.

'edits1' generates all strings obtainable by a single letter edit
from 's' and calls 'cb' with it along with the length of the generated
string and the user-supplied state 'st'. 

The string passed to 'cb' is owned by 'edits1'. 
'cb' must make a copy if it wants to hold on to it

'deletes', 'inserts', 'replaces' and 'transposes' are the
component transformations of 'edits1' and have the same interface.

'edits2' generates all two-letter edits from the input string
by calling edits1 recursively.
*/

static void deletes(const unsigned char *s, size_t len,Callback f,void *st)
{
  unsigned char *t=nv_alloc(len);
  for (size_t n=0;n<len;++n){
    for (size_t i=0;i<n;i++)
      t[i]=s[i];
    for (size_t i=n+1;i<=len;i++)
      t[i-1]=s[i];
    f(st,t,len-1);
  }
  free(t);
}

static void inserts(const unsigned char *s, size_t len, Callback f,void *st)
{
  unsigned char *t=nv_alloc(len+2);
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

static void replaces(const unsigned char *s, size_t len, Callback f,void *st)
{
  unsigned char *t=nv_alloc(len+1);
  memcpy(t,s,len+1);
  for (size_t n=0;n<len;++n){
    for (size_t i=0;i<NLETTERS;i++){
      t[n] = alphabet[i];
      f(st,t,len);
    }
    t[n] = s[n];
  }
  free(t);
}

static void transposes(const unsigned char *s, size_t len, Callback f,void *st)
{
  unsigned char *t=nv_alloc(len+1);
  memcpy(t,s,len+1);
  for (size_t n=0;n<len-1;++n){
    unsigned char c = t[n];
    t[n] = t[n+1];
    t[n+1] = c;
    f(st,t,len);
    c = t[n];
    t[n]=t[n+1];
    t[n+1]=c;
  }
  free(t);
}


void edits1(const unsigned char *word,size_t len,Callback f,void *st)
{
  deletes(word,len,f,st);
  replaces(word,len,f,st);
  transposes(word,len,f,st);
  inserts(word,len,f,st);
}

/*
'edits2' is implemented by saving the original callback
function and state in a 'struct twostate' and then calling
'edits1' with our own callback 'editer2' which calls 
'edits1' a second time with the original callback.
*/

struct twostate
{
  Callback f;
  void *st;
};

static void editer2(void *p,unsigned char *s,size_t len){
  struct twostate *pp = p;
  edits1(s,len,pp->f,pp->st);
}

void edits2(const unsigned char *word,size_t len,Callback f,void *st){
  struct twostate s2 = {f,st};
  edits1(word,len,editer2,&s2);
}


