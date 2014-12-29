#ifndef __JMOY_UTIL__
#define __JMOY_UTIL__

/* A checked malloc. exit(1) on failure */
void *nv_alloc(size_t);

/* strdup */
unsigned char *nv_dup(const unsigned char *,size_t);
#endif
