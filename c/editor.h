#ifndef __JMOY_EDITOR__
#define __JMOY_EDITOR__

typedef void (*Callback)(void * state,char *s,size_t len);

void edits1(const char *word,size_t len,Callback f,void *state);
void edits2(const char *word,size_t len,Callback f,void *state);
 

#endif
