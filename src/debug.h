#ifndef _SIMPLE_LISP_DEBUG_H
#define _SIMPLE_LISP_DEBUG_H

#include <stdio.h>

// Always useful; (nearly) no overhead if never called.
#define debugloc() fprintf(stderr, "%s:%s:%d: ", __FILE__, __func__, __LINE__)

#ifdef _WDEBUG
#define dprintf(...) {debugloc(); fprintf(stderr, __VA_ARGS__);}
#define dputln(str) {debugloc(); fprintf(stderr, "%s\n", str);}
#define eprintf(...) dprintf(__VA_ARGS__)
#else /* _WDEBUG */
#define dprintf(str) 
#define dputln(str)
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#endif /* _WDEBUG */

// Always defer to eprintf...
#define eputln(str) eprintf("%s\n", str)

// A deprecated alias for dprintf.
#define debugf(...) dprintf(__VA_ARGS__)

#endif /* _SIMPLE_LISP_DEBUG_H */
