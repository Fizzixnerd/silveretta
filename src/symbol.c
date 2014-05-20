#include "symbol.h"

#include "types.h"

#include <stdlib.h>

val_symbol mk_val_symbol(string s) {
  return s;
}

void del_val_symbol(val_symbol sym) {
  free(sym);
}
