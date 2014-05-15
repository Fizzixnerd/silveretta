#include "string-helpers.h"
#include "val.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char* ltoa(val_long val) {
  // Allocate and return the string representation of lval_long val.
  // Adapted from:
  // http://stackoverflow.com/questions/2709713/how-to-convert-unsigned-long-to-string
  const int n = snprintf(NULL, 0, "%li", val);
  assert(n > 0);
  char* buf = malloc(sizeof(char) * (n+1));
  int c = snprintf(buf, n+1, "%li", val);
  assert(buf[n] == '\0');
  assert(c == n);
  return buf;
}

