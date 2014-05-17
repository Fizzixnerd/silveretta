#include "eval.h"
#include "val.h"
#include "types.h"
#include "debug.h"
#include "print.h"
#include "error.h"
#include "types.h"

#include "mpc.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

ag_val* ag_eval(ag_val* sexp) {
  if (!sexp) return NULL;
  switch (sexp->type) {
  case AG_TYPE_ERR:
    return ag_eval_err(sexp);
    break;
  case AG_TYPE_LONG:
    return ag_eval_long(sexp);
    break;
  case AG_TYPE_SYMBOL:
    return ag_eval_symbol(sexp);
    break;
  case AG_TYPE_LIST:
    return ag_eval_list(sexp);
    break;
  default:
    return make_ag_val_err(make_val_err(strdup("Type error.")));
  }
}

ag_val* ag_eval_long(ag_val* v) {
  assert(v->type == AG_TYPE_LONG);
  return v;
}

ag_val* ag_eval_symbol(ag_val* v) {
  assert(v->type == AG_TYPE_SYMBOL);
  return v;
}

ag_val* ag_eval_err(ag_val* v) {
  assert(v->type == AG_TYPE_ERR);
  return v;
}

ag_val* ag_eval_list(ag_val* v) {
  assert(v->type == AG_TYPE_LIST);

  if (!v->val.List) return NULL;
  ag_val* head = v->val.List->head;
  ag_val* tail = v->val.List->tail;
  assert(head->type == AG_TYPE_SYMBOL);
  return ag_builtin_op(head->val.Symbol, tail);
}

ag_val* ag_builtin_op(val_symbol sym, ag_val* list) {
  assert(list->type == AG_TYPE_LIST);
  if (!strcmp(sym, "+")) {
    return ag_plus(list);
  } else if (!strcmp(sym, "-")) {
    return ag_minus(list);
  } else if (!strcmp(sym, "*")) {
    return ag_multiply(list);
  } else if (!strcmp(sym, "/")) {
    return ag_divide(list);
  } else {
    string err_msg;
    asprintf(&err_msg, "Undefined symbol %s.", sym);
    return make_ag_val_err(make_val_err(err_msg));
  }
}

ag_val* ag_builtin_op_assert_valid_list(val_symbol op_sym, ag_val* list) {
  assert(list->type == AG_TYPE_LIST);
  val_list* vl = list->val.List;
  if (!vl) {
    string err_msg;
    asprintf(&err_msg, "%s requires at least one operand, 0 given.", op_sym);
    return make_ag_val_err(make_val_err(err_msg));
  } else if (!vl->tail) {
    assert(vl->head);
    if (vl->head->type == AG_TYPE_ERR) return vl->head;
    return NULL;
  } else {
    return ag_builtin_op_assert_valid_list(op_sym, vl->tail);
  }
}

val_long ag_plus_calc(ag_val* list) {
  val_list* vl = list->val.List;
  ag_val* head_val = ag_eval(vl->head);
  assert(head_val->type == AG_TYPE_LONG);
  if (vl->tail) {
    return head_val->val.Long + ag_plus_calc(vl->tail);
  } else {
    return head_val->val.Long;
  }
}

ag_val* ag_plus(ag_val* list) {
  ag_val* assert_result = ag_builtin_op_assert_valid_list("+", list);
  if (assert_result) {
    return assert_result;
  } else {
    return make_ag_val_long(ag_plus_calc(list));
  }
}

val_long ag_minus_calc(ag_val* list) {
  val_list* vl = list->val.List;
  ag_val* head_val = ag_eval(vl->head);
  assert(head_val->type == AG_TYPE_LONG);
  if (vl->tail) {
    // add all the tails and subtract.
    return head_val->val.Long - ag_plus_calc(vl->tail);
  } else {
    // this is okay because ag_minus_calc doesn't recurse.
    return -head_val->val.Long;
  }
}

ag_val* ag_minus(ag_val* list) {
  ag_val* assert_result = ag_builtin_op_assert_valid_list("-", list);
  if (assert_result) {
    return assert_result;
  } else {
    return make_ag_val_long(ag_minus_calc(list));
  }
}

val_long ag_multiply_calc(ag_val* list) {
  val_list* vl = list->val.List;
  ag_val* head_val = ag_eval(vl->head);
  assert(head_val->type == AG_TYPE_LONG);
  if (vl->tail) {
    return head_val->val.Long * ag_multiply_calc(vl->tail);
  } else {
    return head_val->val.Long;
  }
}

ag_val* ag_multiply(ag_val* list) {
  ag_val* assert_result = ag_builtin_op_assert_valid_list("*", list);
  if (assert_result) {
    return assert_result;
  } else {
    return make_ag_val_long(ag_multiply_calc(list));
  }
}

val_long ag_divide_calc(ag_val* list) {
  val_list* vl = list->val.List;
  ag_val* head_val = ag_eval(vl->head);
  assert(head_val->type == AG_TYPE_LONG);
  if (vl->tail) {
    return head_val->val.Long / ag_multiply_calc(vl->tail);
  } else {
    return 1 / head_val->val.Long;
  }
}

ag_val* ag_divide(ag_val* list) {
  ag_val* assert_result = ag_builtin_op_assert_valid_list("/", list);
  if (assert_result) {
    return assert_result;
  } else {
    return make_ag_val_long(ag_divide_calc(list));
  }
}
