#include "eval.h"
#include "lval.h"
#include "debug.h"
#include "errors.h"
#include "types.h"

#include "mpc.h"

#include <stdio.h>
#include <string.h>

wl_lval wl_eval(mpc_ast_t* ast) {
  dprint("AST tag '%s'\n", ast->tag);
  if (strstr(ast->tag, "long_num")) {
    return wl_eval_long(ast);
  }else if (strstr(ast->tag, "sexpr")) {
    return wl_eval_sexpr(ast);
  }else if (strstr(ast->tag, "lisp_symbol")) {
    // TODO: Implement this stuff, and remove the messages when you do.
    eputln("Error: symbol lookup not yet supported.");
    return make_wl_lval_err(FEATURE_INCOMPLETE_ERR);
  }else if (!strcmp(ast->tag, ">")) {
    // TODO Think about this conditional and consider puting it in a
    // one-time called function instead.
    return wl_eval(ast->children[1]);
  } 
  eputln("Error: You should never see this message.");
  return make_wl_lval_err(UNREACHABLE_ERR);
}

wl_lval wl_eval_long(mpc_ast_t* ast) {
  return make_wl_lval_long(atol(ast->contents));
}

wl_lval wl_eval_sexpr(mpc_ast_t* ast) {
  symbol sym = ast->children[1]->contents;
  // FIXME assume there is never '(' <symbol> ')'
  wl_lval x = wl_eval(ast->children[2]);

  // NOTE This is guaranteed to work for at least i==3 because of the
  // ')' character.
  int i = 3;
  while (strstr(ast->children[i]->tag, "expr")) {
    x = wl_apply_function(x, sym, wl_eval(ast->children[i]));
    ++i;
  }
  return x;
}

wl_lval wl_apply_function(wl_lval x, symbol sym, wl_lval y) {
  wl_lval ret;
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
      return make_wl_lval_long(res);
    } else if (!strcmp(sym, "-")) {
      res = x.val.Long - y.val.Long;
      return make_wl_lval_long(res);
    } else if (!strcmp(sym, "*")) {
      res = x.val.Long * y.val.Long;
      return make_wl_lval_long(res);
    } else if (!strcmp(sym, "/")) {
      if (y.val.Long != 0) {
	res = x.val.Long / y.val.Long;
	return make_wl_lval_long(res);
      } else {
	return make_wl_lval_err(DIV_ZERO_ERR);
      }
    } else {
      return make_wl_lval_err(SYMBOL_UNDEFINED_ERR);
    }
  }
}
