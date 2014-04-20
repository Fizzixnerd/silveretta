#include "debug.h"
#include "eval.h"

#include "mpc.h"

#include <stdio.h>
#include <string.h>

long wl_eval(mpc_ast_t* ast) {
  dprintf("AST tag '%s'\n", ast->tag);
  if (strstr(ast->tag, "number")) {
    dputln("Calling wl_eval_number.");
    return wl_eval_number(ast);
  }else if (strstr(ast->tag, "sexpr")) {
    dputln("Calling wl_eval_sexpr.");
    return wl_eval_sexpr(ast);
  }else if (strstr(ast->tag, "symbol")) {
    // TODO: Implement this stuff, and remove the messages when you do.
    dputln("Calling wl_eval_symbol.");
    dputln("Except it doesn't exist yet...");
    eputln("Error: symbol lookup not yet supported.");
    return 0;
  }else if (!strcmp(ast->tag, ">")) {
    // TODO Think about this conditional and consider puting it in a
    // onetime called function instead.
    dputln("Found start of input.");
    return wl_eval(ast->children[1]);
  } 
  eputln("Error: You should never see this message.");
  return 0;
}

long wl_eval_number(mpc_ast_t* ast) {
  return atoi(ast->contents);
}

long wl_eval_sexpr(mpc_ast_t* ast) {
  char* symbol = ast->children[1]->contents;
  //FIXME assume there is never '(' <symbol> ')'
  long x = wl_eval(ast->children[2]);

  //NOTE This is guaranteed to for at least i==3 because of the ')' character.
  int i = 3;
  while (strstr(ast->children[i]->tag, "expr")) {
    x = wl_apply_function(x, symbol, wl_eval(ast->children[i]));
    ++i;
  }
  dputln("Returning.")
  return x;
}

long wl_apply_function(long x, char* symbol, long y) {
  if (!strcmp(symbol, "+")) { return x + y; }
  else if (!strcmp(symbol, "-")) { return x - y; }
  else if (!strcmp(symbol, "*")) { return x * y; }
  else if (!strcmp(symbol, "/")) { return x / y; }
  eputln("Error: You should never see this message.");
  return 0;
}
