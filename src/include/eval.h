#pragma once

#include "val.h"
#include "types.h"

#include "mpc.h"

ag_val* ag_eval(ag_val* sexp);
ag_val* ag_eval_long(ag_val* v);
ag_val* ag_eval_symbol(ag_val* v);
ag_val* ag_eval_err(ag_val* v);
ag_val* ag_eval_list(ag_val* v);

ag_val* ag_builtin_op(val_symbol sym, ag_val* list);
ag_val* ag_builtin_op_assert_valid_list(val_symbol op_sym, ag_val* list);

ag_val* ag_plus(ag_val* list);
ag_val* ag_minus(ag_val* list);
ag_val* ag_multiply(ag_val* list);
ag_val* ag_divide(ag_val* list);


