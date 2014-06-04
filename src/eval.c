#include "eval.h"

#include "val.h"
#include "bool.h"
#include "list.h"
#include "types.h"
#include "debug.h"
#include "print.h"
#include "error.h"
#include "variable.h"

#include "mpc.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

ag_val* ag_eval(ag_val* sexp, env* e) {
  if (!sexp) return NULL;
  switch (sexp->type) {
  case AG_TYPE_ERR:
    return ag_eval_err(sexp, e);
    break;
  case AG_TYPE_LONG:
    return ag_eval_long(sexp, e);
    break;
  case AG_TYPE_SYMBOL:
    return ag_eval_symbol(sexp, e);
    break;
  case AG_TYPE_LIST:
    return ag_eval_list(sexp, e);
    break;
  default:
    ;  // I don't fucking know anymore.
    string err_msg;
    string sexp_str = ag_asprint(sexp);
    asprintf(&err_msg, "%s", sexp_str);
    return mk_ag_val_err(mk_val_err(err_msg));
  }
}

ag_val* ag_eval_long(ag_val* v, env* e) {
  assert(v->type == AG_TYPE_LONG);
  return v;
}

ag_val* ag_eval_symbol(ag_val* v, env* e) {
  assert(v->type == AG_TYPE_SYMBOL);
  bind* found = ag_lex_find(e, v);
  if (found) {
    return found->val;
  } else {
    string err_msg;
    asprintf(&err_msg, "Undefined symbol %s.", v->val.Symbol);
    return mk_ag_val_err(mk_val_err(err_msg));
  }
}

ag_val* ag_eval_err(ag_val* v, env* e) {
  assert(v->type == AG_TYPE_ERR);
  return v;
}

ag_val* ag_eval_list(ag_val* v, env* e) {
  assert(v->type == AG_TYPE_LIST);

  if (!v) return NULL;
  ag_val* head = v->val.List->head;
  ag_val* tail = v->val.List->tail;
  if (head && head->type == AG_TYPE_SYMBOL) {
    return ag_try_builtin(head, tail, e);
  } else {
    string err_msg;
    string list_str = ag_asprint(v);
    asprintf(&err_msg, "head of list %s is not a symbol.", list_str);
    free(list_str);
    return mk_ag_val_err(mk_val_err(err_msg));
  }
}

ag_val* ag_try_builtin(ag_val* sym, ag_val* list, env* e) {
  // TODO: Fix these so that they are runtime errors, not
  // asssert'n'abort errors.
  assert(list->type == AG_TYPE_LIST);
  assert(sym->type == AG_TYPE_SYMBOL);
  val_symbol symbol = sym->val.Symbol;
  if (!strcmp(symbol, "+")) {
    return ag_plus(list, e);
  } else if (!strcmp(symbol, "-")) {
    return ag_minus(list, e);
  } else if (!strcmp(symbol, "*")) {
    return ag_multiply(list, e);
  } else if (!strcmp(symbol, "/")) {
    return ag_divide(list, e);
  } else if (!strcmp(symbol, "let")) {
    return ag_let(list, e);
  } else {
    string err_msg;
    asprintf(&err_msg, "Undefined symbol %s.", symbol);
    return mk_ag_val_err(mk_val_err(err_msg));
  }
}

ag_val* ag_let(ag_val* list, env* e) {
  // looks like (let (x gets 7) <sexp>)
  // so that list == ((x gets 7) <sexp>)

  // TODO: Make these asserts runtime errors!
  assert(list->type == AG_TYPE_LIST);
  ag_val* gets_list = list->val.List->head;
  assert(gets_list);
  assert(gets_list->type == AG_TYPE_LIST);
  ag_val* bound_symbol = gets_list->val.List->head;
  assert(bound_symbol);
  assert(bound_symbol->type == AG_TYPE_SYMBOL);
  ag_val* gets_str = gets_list->val.List->tail->val.List->head;
  assert(gets_str);
  assert(!strcmp(gets_str->val.Symbol, GETS_STR));
  ag_val* bound_val = gets_list->val.List->tail->val.List->tail->val.List->head;
  assert(bound_val);
  ag_val* end_of_list = gets_list->val.List->tail->val.List->tail->val.List->tail;
  assert(!end_of_list);

  assert(list->val.List->tail);
  ag_val* wrapped_sexp = list->val.List->tail->val.List->head;
  
  bind* b = mk_bind(bound_val, bound_symbol, false);
  ag_lex_push(e, b);
  return ag_eval(wrapped_sexp, e);
}

ag_val* ag_if(ag_val* list, env* e) {
  // looks like (if <condition> (then <then-sexp>) [ (else <else-sexp>) ])
  // so list == (<cond> (then <tsexp>) [ (else <esexp>) ])
  // or list == (<cond> <sexp1> [ <sexp2> ])

  // TODO: Make these asserts runtime errors!
  assert(list->type == AG_TYPE_LIST);
  ag_val* cond = ag_head(list);
  assert(cond);
  ag_val* sexp1 = ag_head(ag_tail(list));
  assert(sexp1);
  ag_val* sexp2 = ag_tail(ag_tail(list)) ?
                  ag_head(ag_tail(ag_tail(list))) :
                  NULL ;
  assert(!ag_tail(ag_tail(ag_tail(list))));

  ag_val* cond_val = ag_to_bool(ag_eval(cond, e), e);
  ag_val* tsexp = ag_head(ag_tail(sexp1));
  ag_val* esexp = sexp2 ? ag_head(ag_tail(sexp2)) :
                          NULL ;
  assert(cond_val->type == AG_TYPE_BOOL);
  
  if (cond_val->val.Bool) {
    return ag_eval(tsexp, e);
  } else {
    return ag_eval(esexp, e);
  }
}

ag_val* ag_while(ag_val* list, env* e) {
  // looks like (while <condition> [ <sexp>* ])
  // so list == (<cond> . <sexps>)

  ag_val* cond  = ag_head(list);
  ag_val* sexps = ag_tail(list);
  ag_val* cond_val = ag_to_bool(cond, e);
  ag_val* result = NULL;
  while (cond_val) {
    result = ag_do(sexps, e);
  }
  return result;
}

ag_val* ag_do(ag_val* list, env* e) {
  // looks like (do <sexp>*)
  // so list == (<sexp>*)
  ag_val* it;
  ag_val* result;
  for (it = ag_head(list); it; it = ag_tail(it)) {
    result = ag_eval(it, e);
  }
  return result;
}

ag_val* ag_quote(ag_val* list, env* e) {
  // looks like (quote <sexp>)
  // so list == (<sexp>)

  assert(ag_tail(list) == NULL);
  return ag_head(list);
}


ag_val* ag_set(ag_val* list, env* e) {
  // looks like (set <sym-sexp> <- <val-sexp>)
  // so list == (<ssexp> <- <vsexp>)

  ag_val* ssexp = ag_head(list);
  ag_val* sym;
  if (ssexp->type == AG_TYPE_SYMBOL) {
    sym = ssexp;
  } else {
    sym = ag_eval(ssexp, e);
  }
  ag_val* vsexp = ag_head(ag_tail(ag_tail(list)));
  ag_val* vsexp_val = ag_eval(vsexp, e);
    
  // FIXME: Learn how dynamic variables actually work.
  bind* found = ag_lex_find(e, sym);
  if (found) {
    assert(!found);
    return NULL;
  } else {
    ag_lex_add(e, mk_bind(vsexp_val, sym, true));
    return vsexp_val;
  }
}

ag_val* ag_setBang(ag_val* list, env* e) {
  // looks like (set! <sym-sexp> <- <val-sexp>)
  // so list == (<ssexp> <- <vsexp>)

  ag_val* ssexp = ag_head(list);
  ag_val* sym;
  if (ssexp->type == AG_TYPE_SYMBOL) {
    sym = ssexp;
  } else {
    sym = ag_eval(ssexp, e);
  }
  ag_val* vsexp = ag_head(ag_tail(ag_tail(list)));
  ag_val* vsexp_val = ag_eval(vsexp, e);

  bind* found = ag_lex_find(e, sym);
  if (found) {
    ag_lex_replace(e, mk_bind(vsexp_val, sym, false));
  } else {
    ag_set(list, e);
  }
  return vsexp_val;
}

ag_val* ag_builtin_op_assert_valid_list(val_symbol op_sym, ag_val* list) {
  assert(list->type == AG_TYPE_LIST);
  val_list* vl = list->val.List;
  if (!vl) {
    string err_msg;
    asprintf(&err_msg, "%s requires at least one operand, 0 given.", op_sym);
    return mk_ag_val_err(mk_val_err(err_msg));
  } else if (!vl->tail) {
    assert(vl->head);
    if (vl->head->type == AG_TYPE_ERR) return vl->head;
    return NULL;
  } else {
    return ag_builtin_op_assert_valid_list(op_sym, vl->tail);
  }
}

val_long ag_plus_calc(ag_val* list, env* e) {
  val_list* vl = list->val.List;
  ag_val* head_val = ag_eval(vl->head, e);
  assert(head_val->type == AG_TYPE_LONG);
  if (vl->tail) {
    return head_val->val.Long + ag_plus_calc(vl->tail, e);
  } else {
    return head_val->val.Long;
  }
}

ag_val* ag_plus(ag_val* list, env* e) {
  ag_val* assert_result = ag_builtin_op_assert_valid_list("+", list);
  if (assert_result) {
    return assert_result;
  } else {
    return mk_ag_val_long(ag_plus_calc(list, e));
  }
}

val_long ag_minus_calc(ag_val* list, env* e) {
  val_list* vl = list->val.List;
  ag_val* head_val = ag_eval(vl->head, e);
  assert(head_val->type == AG_TYPE_LONG);
  if (vl->tail) {
    // add all the tails and subtract.
    return head_val->val.Long - ag_plus_calc(vl->tail, e);
  } else {
    // this is okay because ag_minus_calc doesn't recurse.
    return -head_val->val.Long;
  }
}

ag_val* ag_minus(ag_val* list, env* e) {
  ag_val* assert_result = ag_builtin_op_assert_valid_list("-", list);
  if (assert_result) {
    return assert_result;
  } else {
    return mk_ag_val_long(ag_minus_calc(list, e));
  }
}

val_long ag_multiply_calc(ag_val* list, env* e) {
  val_list* vl = list->val.List;
  ag_val* head_val = ag_eval(vl->head, e);
  assert(head_val->type == AG_TYPE_LONG);
  if (vl->tail) {
    return head_val->val.Long * ag_multiply_calc(vl->tail, e);
  } else {
    return head_val->val.Long;
  }
}

ag_val* ag_multiply(ag_val* list, env* e) {
  ag_val* assert_result = ag_builtin_op_assert_valid_list("*", list);
  if (assert_result) {
    return assert_result;
  } else {
    return mk_ag_val_long(ag_multiply_calc(list, e));
  }
}

val_long ag_divide_calc(ag_val* list, env* e) {
  val_list* vl = list->val.List;
  ag_val* head_val = ag_eval(vl->head, e);
  assert(head_val->type == AG_TYPE_LONG);
  if (vl->tail) {
    return head_val->val.Long / ag_multiply_calc(vl->tail, e);
  } else {
    return 1 / head_val->val.Long;
  }
}

ag_val* ag_divide(ag_val* list, env* e) {
  ag_val* assert_result = ag_builtin_op_assert_valid_list("/", list);
  if (assert_result) {
    return assert_result;
  } else {
    return mk_ag_val_long(ag_divide_calc(list, e));
  }
}
