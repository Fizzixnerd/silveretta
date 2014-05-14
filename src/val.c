#include "val.h"
#include "macros.h"
#include "string-helpers.h"
#include "debug.h"

#include <string.h>

ag_val make_ag_val_long(lval_long x) {
  ag_val v = {.type = LTYPE_LONG, .val.Long = x};
  return v;
}

ag_val make_ag_val_err(lval_err e) {
  ag_val v = {.type = LTYPE_ERR, .val.Error = e};
  return v;
}

char* val_to_string(ag_val v) {
  // FIXME : Finish this.
  // The caller must free the string.
  switch (v.type) {
  case AG_TYPE_LONG:
    return ltoa(v.val.Long);
    break;
  case AG_TYPE_ERR:
    return astrcpy(v.val.Error.msg);
    break;
  default:
    #include "stringify.h"
    eprintf("Error: val_to_string doesn't know how to stringify type %s.", STRINGIFY(v.type));
    #include "stringify-cleanup.h"
    return "Error: Can't stringify the lval given to lval_to_string.";
  }
}
