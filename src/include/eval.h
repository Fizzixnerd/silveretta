#pragma once

#include "val.h"
#include "types.h"

#include "mpc.h"

ag_val wl_eval(mpc_ast_t* ast);
ag_val wl_eval_long(mpc_ast_t* ast);
ag_val wl_eval_sexpr(mpc_ast_t* ast);
ag_val wl_apply_function(ag_val x, ag_symbol sym, ag_val y);

