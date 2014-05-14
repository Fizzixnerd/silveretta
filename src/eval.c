#include "eval.h"
#include "lval.h"
#include "debug.h"
#include "errors.h"
#include "types.h"

#include "mpc.h"

#include <stdio.h>
#include <string.h>

ag_val ag_eval(mpc_ast_t* ast) {
  dprint("AST tag '%s'\n", ast->tag);
  if (strstr(ast->tag, "long_num")) {
    return ag_eval_long(ast);
  }else if (strstr(ast->tag, "sexpr")) {
    return ag_eval_sexpr(ast);
  }else if (strstr(ast->tag, "lisp_symbol")) {
    // TODO: Implement this stuff, and remove the messages when you do.
    eputln("Error: symbol lookup not yet supported.");
    return make_ag_val_err(FEATURE_INCOMPLETE_ERR);
  }else if (!strcmp(ast->tag, ">")) {
    // TODO Think about this conditional and consider puting it in a
    // one-time called function instead.
    return ag_eval(ast->children[1]);
  } 
  eputln("Error: You should never see this message.");
  return make_ag_val_err(UNREACHABLE_ERR);
}

ag_val ag_eval_long(mpc_ast_t* ast) {
  return make_ag_lval_long(atol(ast->contents));
}

ag_val ag_eval_sexpr(mpc_ast_t* ast) {
  symbol sym = ast->children[1]->contents;
  // FIXME assume there is never '(' <symbol> ')'
  ag_val x = ag_eval(ast->children[2]);

  // NOTE This is guaranteed to work for at least i==3 because of the
  // ')' character.
  int i = 3;
  while (strstr(ast->children[i]->tag, "expr")) {
    x = ag_apply_function(x, sym, ag_eval(ast->children[i]));
    ++i;
  }
  return x;
}

ag_val ag_apply_function(ag_val x, symbol sym, ag_val y) {
  ag_val ret;
  // return x or y if they are an error.
  if (!x.type) {
    return x;
  } else if (!y.type) {
    return y;
  } else {
    // neither x nor y are errors.
    lval_long res;
    if (!strcmp(sym, "+")) {
      res = x.val.Long + y.val.Long;
      return make_ag_val_long(res);
    } else if (!strcmp(sym, "-")) {
      res = x.val.Long - y.val.Long;
      return make_ag_val_long(res);
    } else if (!strcmp(sym, "*")) {
      res = x.val.Long * y.val.Long;
      return make_ag_val_long(res);
    } else if (!strcmp(sym, "/")) {
      if (y.val.Long != 0) {
	res = x.val.Long / y.val.Long;
	return make_ag_val_long(res);
      } else {
	return make_ag_val_err(DIV_ZERO_ERR);
      }
    } else {
      return make_ag_val_err(SYMBOL_UNDEFINED_ERR);
    }
  }
}