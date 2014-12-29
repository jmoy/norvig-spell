#ifndef __JMOY_EDITOR__
#define __JMOY_EDITOR__

typedef void (*Callback)(void * state,unsigned char *s,size_t len);

/*
'edits1' generates all strings obtainable by a single letter edit
from 's' (which is assumed to be of length 'len')
and calls 'f' with it along with the length of the generated
string and the user-supplied state 'state'. 

The string passed to 'f' is owned by 'edits1'. 
'f' must make a copy if it wants to hold on to it
*/

void edits1(const unsigned char *word,size_t len,Callback f,void *state);

/*'edits2' has the same interface as 'edits1' by generates all
two-letter edits by calling 'edits1' recursively.
*/

void edits2(const unsigned char *word,size_t len,Callback f,void *state);
 

#endif
