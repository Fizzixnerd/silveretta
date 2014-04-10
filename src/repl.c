#include "include/linedit.h"

#include <stdio.h>

int main(int argc, char** argv) {
  puts("WalkerLisp Version 0.0.0.0.2");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    char * input = readline("lisp> ");
    add_history(input);
    
    printf("No you're a %s!\n", input);

    free(input);
  }

  return 0;
}

