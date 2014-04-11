#include "include/linedit.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  puts("WalkerLisp Version 0.0.0.0.2");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    char* prompt = "wlisp> ";
    char* input = readline(prompt);
    add_history(input);
    
    printf("No you're a %s!\n", input);

    free(input);
  }

  return EXIT_SUCCESS;
}
