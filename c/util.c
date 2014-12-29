#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

void *nv_alloc(size_t sz)
{
  void *p = malloc(sz);
  if (p==NULL){
    fputs("Memory allocation failed.",stderr);
    exit(1);
  }
  return p;
}

unsigned char *nv_dup(const unsigned char *s,size_t len)
{
  unsigned char *t;

  t = nv_alloc(len+1);
  memcpy(t,s,len+1);
  return t;
}
  
