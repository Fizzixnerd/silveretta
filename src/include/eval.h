#pragma once

#include "val.h"
#include "types.h"
#include "variable.h"

#include "mpc.h"

#define GETS_STR "gets"

ag_val* ag_eval(ag_val* sexp, env* e);
ag_val* ag_eval_long(ag_val* v, env* e);
ag_val* ag_eval_symbol(ag_val* v, env* e);
ag_val* ag_eval_err(ag_val* v, env* e);
ag_val* ag_eval_list(ag_val* v, env* e);

ag_val* ag_try_builtin(ag_val* sym, ag_val* list, env* e);
ag_val* ag_builtin_op_assert_valid_list(val_symbol op_sym, ag_val* list);

ag_val* ag_let(ag_val* list, env* e);
ag_val* ag_plus(ag_val* list, env* e);
ag_val* ag_minus(ag_val* list, env* e);
ag_val* ag_multiply(ag_val* list, env* e);
ag_val* ag_divide(ag_val* list, env* e);


