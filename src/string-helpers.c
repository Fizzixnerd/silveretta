#include "string-helpers.h"
#include "lval.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char* ltoa(lval_long val) {
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

char* astrcpy(const char* s) {
  // Allocate and return a copy of string s.
  const int n = snprintf(NULL, 0, "%s", s);
  assert(n > 0);
  char* buf = malloc(sizeof(char) * (n+1));
  int c = snprintf(buf, n+1, "%s", s);
  assert(buf[n] == '\0');
  assert(c == n);
  return buf;
}
