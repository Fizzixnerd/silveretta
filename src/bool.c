#include "bool.h"
#include "types.h"

ag_val* ag_to_bool(ag_val* v, env* e) {
  return mk_ag_val_bool(ag_to_val_bool(v, e));
}

val_bool ag_to_val_bool (ag_val* v, env* e) {
  switch (v->type) {
  case AG_TYPE_LONG:
    return (v->val.Long != 0);
    break;
  case AG_TYPE_BOOL:
    return v->val.Bool;
    break;
  case AG_TYPE_LIST:
    return (v->val.List != NULL);
    break;
  case AG_TYPE_ERR:
    return false;
    break;
  case AG_TYPE_SYMBOL:
    if (!strcmp(v->val.Symbol, "true")) {
      return true;
    } else if (!strcmp(v->val.Symbol, "false")) {
      return false;
    } else {
      return true;
    }
    break;
  default:
    return true;
    break;
  }
}
    
