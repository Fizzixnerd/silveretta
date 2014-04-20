#include "debug.h"
#include "linedit.h"
#include "eval.h"

#include "mpc.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  mpc_parser_t* wlisp = mpc_new("wlisp");
  mpc_parser_t* sexpr = mpc_new("sexpr");
  mpc_parser_t* expr = mpc_new("expr");
  mpc_parser_t* symbol = mpc_new("symbol");
  mpc_parser_t* number = mpc_new("number");

  mpca_lang(MPC_LANG_DEFAULT,
	    "                                                   \
             number   : /-?[0-9]+/ ;                            \
             symbol   : '+' | '-' | '*' | '/' ;                 \
             sexpr    : '(' <symbol> <expr>* ')' ;              \
             expr     : <number> | <symbol> | <sexpr> ;         \
             wlisp    : /^/ <expr>* /$/ ;                       \
            ",
	    number, symbol, sexpr, expr, wlisp);

  puts("WalkerLisp Version 1.0");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    char* prompt = "wlisp> ";
    char* input = readline(prompt);
    add_history(input);

    dputln("History added.");
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, wlisp, &r)) {
      mpc_ast_print(r.output);
      /* Print AST on success. */
      dputln("Got the AST.");
      long result = wl_eval(r.output);
      dputln("Eval returned.");
      printf("%li\n", result);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise print the error. */
      dputln("There was an error with the AST.")
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
    dputln("input was freed.");
  }

  mpc_cleanup(5, number, symbol, sexpr, expr, wlisp);
  dputln("Parsers were freed.");
  return EXIT_SUCCESS;
}
