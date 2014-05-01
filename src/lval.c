#include "lval.h"

lval_value lval_new_num(long x) {
  return (lval_value)x;
}

wl_lval lval_num(long x) {
  wl_lval v;
  v.type = LTYPE_NUM;
  v.val = lval_new_num(x);
  return v;
}

wl_lval lval_err(lval_error e) {
  lval v;
  v.type = LTYPE_ERR;
  v.err = e;
  return v;
}

char* lval_to_string(wl_lval v) {
  
}
