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

char *nv_dup(const char *s)
{
  size_t len;
  char *t;

  len = strlen(s);
  t = nv_alloc(len+1);
  strcpy(t,s);
  return t;
}
  
