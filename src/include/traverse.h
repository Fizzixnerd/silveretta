#ifndef _SIMPLE_LISP_TRAVERSE_H
#define _SIMPLE_LISP_TRAVERSE_H

#include "mpc.h"

void wl_ast_traverse(mpc_ast_t* ast, void (*func)(mpc_ast_t*, void*), void* func_state);

#endif
