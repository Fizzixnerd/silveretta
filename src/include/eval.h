#ifndef _SIMPLE_LISP_EVAL_H
#define _SIMPLE_LISP_EVAL_H

#include "lval.h"
#include "types.h"

#include "mpc.h"

wl_lval wl_eval(mpc_ast_t* ast);
wl_lval wl_eval_long(mpc_ast_t* ast);
wl_lval wl_eval_sexpr(mpc_ast_t* ast);
wl_lval wl_apply_function(wl_lval x, symbol sym, wl_lval y);

#endif /* _SIMPLE_LISP_EVAL_H */
