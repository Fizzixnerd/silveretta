#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

static char input[2048];

int main(int argc, char** argv) {
  puts("WalkerLisp Version 0.0.0.0.2");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    fputs("lisp> ", stdout);
    fgets(input, 2048, stdin);
    printf("No you're a %s", input);
  }

  return 0;
}

