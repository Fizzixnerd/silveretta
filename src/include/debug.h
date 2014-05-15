#pragma once

#include <stdio.h>

// Always useful; (nearly) no overhead if never called.
#define debugloc() fprintf(stderr, "%s:%s:%d: ", __FILE__, __func__, __LINE__)

#ifdef AG_DEBUG
#undef AG_DEBUG
#define AG_DEBUG 1
#define dprint(...) {debugloc(); fprintf(stderr, __VA_ARGS__);}
#define dputln(str) {debugloc(); fprintf(stderr, "%s\n", str);}
#define eprintf(...) dprint(__VA_ARGS__)
#else // AG_DEBUG
#undef AG_DEBUG
#define AG_DEBUG 0
#define dprint(...) 
#define dputln(str)
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#endif // AG_DEBUG

// Always defer to eprintf...
#define eputln(str) eprintf("%s\n", str)
