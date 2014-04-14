#ifndef _SIMPLE_LISP_EVAL_H
#define _SIMPLE_LISP_EVAL_H

#include "mpc.h"

long wl_eval(mpc_ast_t* ast);
long wl_eval_number(mpc_ast_t* ast);
long wl_eval_sexpr(mpc_ast_t* ast);
long wl_apply_function(long x, char* symbol, long y);

#endif
