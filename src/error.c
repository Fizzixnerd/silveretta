#include "error.h"
#include "types.h"

// val_err gets ownership of the msg
val_err make_val_err(char* msg) {
  val_err e = {.msg = msg};
  return e;
}
