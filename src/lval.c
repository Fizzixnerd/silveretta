#include "lval.h"
#include "macros.h"
#include "string-helpers.h"
#include "debug.h"

#include <string.h>

wl_lval make_wl_lval_long(lval_long x) {
  wl_lval v = {.type = LTYPE_LONG, .val.Long = x};
  return v;
}

wl_lval make_wl_lval_err(lval_err e) {
  wl_lval v = {.type = LTYPE_ERR, .val.Error = e};
  return v;
}

char* lval_to_string(wl_lval v) {
  // FIXME : Finish this.
  // The caller must free the string.
  switch (v.type) {
  case LTYPE_LONG:
    return ltoa(v.val.Long);
    break;
  case LTYPE_ERR:
    return astrcpy(v.val.Error.msg);
    break;
  default:
    #include "stringify.h"
    eprintf("Error: lval_to_string doesn't know how to stringify type %s.", STRINGIFY(v.type));
    #include "stringify-cleanup.h"
    return "Error: Can't stringify the lval given to lval_to_string.";
  }
}
