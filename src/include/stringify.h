#ifndef _SIMPLE_LISP_STRINGIFY_H
#define _SIMPLE_LISP_STRINGIFY_H

#ifdef STRINGIFY
#warning "You have asked me to define the helper macros in stringify.h more than once without cleaning up afterwards.  Please clean them up after you are done with them by #include-ing \"stringify-cleanup.h\"."
#else /* XSTR || STRINGIFY || CAT */

#define XSTR(str) #str
#define STRINGIFY(str) XSTR(str)
#define CAT(str1, str2) (str1 ## str2)

#endif /* CAT || STRINGIFY */
#endif /* _SIMPLE_LISP_STRINGIFY_H */
