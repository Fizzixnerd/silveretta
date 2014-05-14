#ifndef _SIMPLE_LISP_DEBUG_H
#define _SIMPLE_LISP_DEBUG_H

#include <stdio.h>

// Always useful; (nearly) no overhead if never called.
#define debugloc() fprintf(stderr, "%s:%s:%d: ", __FILE__, __func__, __LINE__)

#ifdef _WDEBUG
#undef _WDEBUG
#define _WDEBUG 1
#define dprint(...) {debugloc(); fprintf(stderr, __VA_ARGS__);}
#define dputln(str) {debugloc(); fprintf(stderr, "%s\n", str);}
#define eprintf(...) dprint(__VA_ARGS__)
#else /* _WDEBUG */
#undef _WDEBUG
#define _WDEBUG 0
#define dprint(...) 
#define dputln(str)
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#endif /* _WDEBUG */

// Always defer to eprintf...
#define eputln(str) eprintf("%s\n", str)

#endif /* _SIMPLE_LISP_DEBUG_H */
