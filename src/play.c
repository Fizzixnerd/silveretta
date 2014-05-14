#include <stdio.h>
#include "play.h"

int expression_fun (void) {
  fun_t fun;
  if ((fun = make_fun(1, "Hello, world!")).flag) {
    puts("flag is 1!");
  }else {
    puts("flag is NOT 1!");
  }
  return fun.flag;
}

fun_t make_fun (int flag, char* data) {
  return (fun_t){flag, data};
}

int main (int argv, char** argc) {
  return 1 - expression_fun();
}
