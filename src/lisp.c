#include "debug.h"
#include "linedit.h"
#include "eval.h"
#include "lval.h"
#include "types.h"

#include "mpc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
  mpc_parser_t* wlisp = mpc_new("wlisp");
  mpc_parser_t* sexpr = mpc_new("sexpr");
  mpc_parser_t* expr = mpc_new("expr");
  mpc_parser_t* lisp_symbol = mpc_new("lisp_symbol");
  mpc_parser_t* long_num = mpc_new("long_num");

  mpca_lang(MPC_LANG_DEFAULT,
	    "                                                   \
             long_num    : /-?[0-9]+/ ;                         \
             lisp_symbol : '+' | '-' | '*' | '/' ;              \
             sexpr    : '(' <lisp_symbol> <expr>* ')' ;         \
             expr     : <long_num> | <lisp_symbol> | <sexpr> ;    \
             wlisp    : /^/ <expr>* /$/ ;                       \
            ",
	    long_num, lisp_symbol, sexpr, expr, wlisp);

  puts("WalkerLisp Version 1.0");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    char* prompt = "lisp>> ";
    char* input = readline(prompt);
    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, wlisp, &r)) {
      mpc_ast_print(r.output);
      wl_lval result = wl_eval(r.output);
      char* result_str = lval_to_string(result);
      printf("%s\n", result_str);
      free(result_str);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise print the error. */
      dputln("There was an error with the AST.")
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    free(input);
  }

  mpc_cleanup(5, long_num, lisp_symbol, sexpr, expr, wlisp);
  return EXIT_SUCCESS;
}
