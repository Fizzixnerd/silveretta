#include "val.h"
#include "types.h"
#include "list.h"
#include "debug.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static ag_val* OOM_ERR;

static ag_val* oom_err() {
  OOM_ERR->type = AG_TYPE_ERR;
  OOM_ERR->val.Error = make_val_err(OUT_OF_MEMORY_ERR);
  return OOM_ERR;
}


ag_val* make_ag_val() {
  return malloc(sizeof(ag_val));
}

ag_val* make_ag_val_long(val_long x) {
  ag_val* v = make_ag_val();
  if (v) {
    v->type = AG_TYPE_LONG;
    v->val.Long = x;
    return v;
  } else {
    return oom_err();
  }
}

ag_val* make_ag_val_err(val_err e) {
  ag_val* v = make_ag_val();
  if (v) {
    v->type = AG_TYPE_ERR;
    v->val.Error = e;
    return v;
  } else {
    return oom_err();
  }
}

ag_val* make_ag_val_bool(val_bool b) {
  ag_val* v = make_ag_val();
  if (v) {
    v->type = AG_TYPE_BOOL;
    v->val.Bool = b;
    return v;
  } else {
    return oom_err();
  }
}

ag_val* make_ag_val_list(val_list* list) {
  ag_val* v = make_ag_val();
  if (v) {
    v->type = AG_TYPE_LIST;
    v->val.List = list;
    return v;
  } else {
    return oom_err();
  }
}

ag_val* make_ag_val_symbol(val_symbol sym) {
  ag_val* v = make_ag_val();
  if (v) {
    v->type = AG_TYPE_SYMBOL;
    v->val.Symbol = sym;
    return v;
  } else {
    return oom_err();
  }
}

void ag_val_del(ag_val *v) {
  if (!v) return;
  
  switch (v->type) {
  case AG_TYPE_LONG:
  case AG_TYPE_BOOL:
    break;
  case AG_TYPE_ERR:
    free(v->val.Error.msg);
    break;
  case AG_TYPE_SYMBOL:
    free(v->val.Symbol);
    break;
  case AG_TYPE_LIST:
    val_list_del(v->val.List);
    break;
  }
  free(v);
  return;
}

