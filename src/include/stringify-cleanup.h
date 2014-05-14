#ifndef _SIMPLE_LISP_STRINGIFY_CLEANUP_H
#define _SIMPLE_LISP_STRINGIFY_CLEANUP_H

#ifndef STRINGIFY
#warning "You have asked me to cleanup the macro definitions in 'include/stringify.h', but these definitions don't exist.  You may want to double check your code, as this is almost certainly an error."
#else /* XSTR || STRINGIFY || CAT */
#undef XSTR
#undef STRINGIFY
#undef CAT

#endif /* XSTR || STRINGIFY || CAT */
#endif /* _SIMPLE_LISP_STRINGIFY_CLEANUP_H */
