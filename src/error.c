#include "error.h"
#include "types.h"

// val_err gets ownership of the msg
val_err mk_val_err(char* msg) {
  val_err e = {.msg = msg};
  return e;
}

void del_val_err(val_err e) {
  free(e.msg);
}
