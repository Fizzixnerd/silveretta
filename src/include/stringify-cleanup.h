#ifndef STRINGIFY
#warning "You have asked me to cleanup the macro definitions in 'include/stringify.h', but these definitions don't exist.  You may want to double check your code, as this is almost certainly an error."
#else // STRINGIFY
#undef XSTR
#undef STRINGIFY
#undef CAT

#endif // STRINGIFY
