#ifndef _SIMPLE_LISP_LINEDIT_H
#define _SIMPLE_LISP_LINEDIT_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
/* Windows doesn't need editline. */

static char buffer[2048];

/* Fake readline */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = "\0";
  return cpy;
}

/* Fake add_history */
void add_history(char* unused) {}

#else /* _WIN32 */

#include <editline/readline.h>
#include <editline/history.h>

#endif /* _WIN32 */

#endif /* _SIMPLE_LISP_LINEDIT_H */
