#include "debug.h"
#include "eval.h"
#include "val.h"
#include "types.h"

#include "mpc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linedit.h>

int main(int argc, char** argv) {
  mpc_parser_t* long_num = mpc_new("long_num");
  mpc_parser_t* symbol = mpc_new("symbol");
  mpc_parser_t* nil = mpc_new("nil");
  mpc_parser_t* list = mpc_new("list");
  mpc_parser_t* sexp = mpc_new("sexp");
  mpc_parser_t* silveretta = mpc_new("silveretta");

  mpca_lang(MPC_LANG_DEFAULT,
	    "                                                   \
             long_num    : /-?[0-9]+/ ;                         \
             symbol : '+' | '-' | '*' | '/' ;              \
             nil :   '(' ')' ;                                \
             list    : '(' <symbol> <sexp>* ')' ;         \
             sexp     : <long_num> | <symbol> | <list> | <nil> ;    \
             silveretta    : /^/ <sexp>* /$/ ;                       \
            ",
	    long_num, symbol, nil, list, sexp, silveretta);

  puts("WalkerLisp Version 1.0");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    char* prompt = "ag>> ";
    char* input = readline(prompt);
    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, silveretta, &r)) {
      mpc_ast_print(r.output);
      ag_val* result = ag_read(r.output);
      char* result_str = ag_val_to_string(result);
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

  mpc_cleanup(6, long_num, symbol, nil, list, sexp, silveretta);
  return EXIT_SUCCESS;
}
