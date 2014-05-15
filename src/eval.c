#include "eval.h"
#include "val.h"
#include "debug.h"
#include "errors.h"
#include "types.h"

#include "mpc.h"

#include <stdio.h>
#include <string.h>

ag_val* ag_eval(ag_val* sexp) {
  ag_print(sexp);
  return sexp;
}

/* ag_val* ag_eval_long(mpc_ast_t* ast) { */
/*   return make_ag_val_long(atol(ast->contents)); */
/* } */

/* ag_val* ag_eval_list(mpc_ast_t* ast) { */
/*   val_symbol sym = ast->children[1]->contents; */
/*   // FIXME assume there is never '(' <symbol> ')' */
/*   ag_val* x = ag_eval(ast->children[2]); */

/*   // NOTE This is guaranteed to work for at least i==3 because of the */
/*   // ')' character. */
/*   int i = 3; */
/*   while (strstr(ast->children[i]->tag, "sexp")) { */
/*     x = ag_apply_function(x, sym, ag_eval(ast->children[i])); */
/*     ++i; */
/*   } */
/*   return x; */
/* } */

/* ag_val* ag_apply_function(ag_val x, ag_symbol sym, ag_val y) { */
/*   // return x or y if they are an error. */
/*   if (!x.type) { */
/*     return x; */
/*   } else if (!y.type) { */
/*     return y; */
/*   } else { */
/*     // neither x nor y are errors. */
/*     val_long res; */
/*     if (!strcmp(sym, "+")) { */
/*       res = x.val.Long + y.val.Long; */
/*       return make_ag_val_long(res); */
/*     } else if (!strcmp(sym, "-")) { */
/*       res = x.val.Long - y.val.Long; */
/*       return make_ag_val_long(res); */
/*     } else if (!strcmp(sym, "*")) { */
/*       res = x.val.Long * y.val.Long; */
/*       return make_ag_val_long(res); */
/*     } else if (!strcmp(sym, "/")) { */
/*       if (y.val.Long != 0) { */
/* 	res = x.val.Long / y.val.Long; */
/* 	return make_ag_val_long(res); */
/*       } else { */
/* 	return make_ag_val_err(DIV_ZERO_ERR); */
/*       } */
/*     } else { */
/*       return make_ag_val_err(SYMBOL_UNDEFINED_ERR); */
/*     } */
/*   } */
/* } */
