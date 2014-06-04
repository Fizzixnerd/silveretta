#include "variable.h"

#include "val.h"
#include "symbol.h"
#include "list.h"
#include "types.h"
#include "debug.h"

#include "uthash.h"

#include <stdlib.h>
#include <assert.h>

bind* mk_bind(ag_val* v, ag_val* sym, bool dyn) {
  assert(sym->type == AG_TYPE_SYMBOL);
  bind* b = malloc(sizeof(bind));
  b->sym = sym->val.Symbol;
  b->val = v;
  b->dyn = dyn;
  return b;
}

env* mk_env() {
  env* e = malloc(sizeof(env));
  e->bhash = malloc(sizeof(bind*));
  *e->bhash = NULL;
  e->global = false;
  e->boundary = true;
  e->next = NULL;
  return e;
}

env* mk_global_env() {
  env* e = mk_env();
  e->global = true;
  return e;
}

void del_bind(bind* b) {
  free(b);
}

void del_env(env* e) {
  bind* b;
  bind* tmp;
  HASH_ITER(hh, *e->bhash, b, tmp) {
    HASH_DEL(*e->bhash, b);
    del_bind(b);
  }
  free(e->bhash);
  free(e);
}

void del_env_chain(env* e) {
  env* it;
  for (it = e; it; it=it->next) {
    del_env(it);
  }
}

void ag_lex_add(env* e, bind* b) {
  HASH_ADD_KEYPTR(hh, *e->bhash, b->sym, strlen(b->sym), b);
}

bind* ag_lex_replace(env* e, bind* b) {
  bind* replaced;
  HASH_REPLACE(hh, *e->bhash, sym, strlen(b->sym), b, replaced);
  return replaced;
}

bind* ag_lex_rm(env* e, bind* b) {
  HASH_DEL(*e->bhash, b);
  return b;
}

// FIXME: This might be bad if things get del'd, because e->bhash
// could change... I think... Maybe...!  Might be okay if we always
// return the new main env*...
bind* ag_lex_find1(env* e, ag_val* sym) {
  assert(sym->type == AG_TYPE_SYMBOL);
  bind* it;
  for (it = *e->bhash; it && strcmp(it->sym, sym->val.Symbol); it = it->hh.next)
    { ; }
  return it;
}
 
bind* ag_lex_find(env* e, ag_val* sym) {
  assert(sym->type == AG_TYPE_SYMBOL);
  bind* b;
  env* it;
  for (it = e; it && !(b = ag_lex_find1(it, sym)) && it->next; it = it->next)
    { ; }
  return b;
}

bind* ag_lex_find_rm(env* e, ag_val* sym) {
  assert(sym->type == AG_TYPE_SYMBOL);

  bind* b = ag_lex_find(e, sym);
  if (b) {
    b = ag_lex_rm(e, b);
  }
  return b;
}

bind* ag_lex_cp_bind(bind* b) {
  if (!b) return NULL;
  bind* cpy = malloc(sizeof(bind));
  cpy->sym = b->sym;
  cpy->val = b->val;
  cpy->dyn = cpy->dyn;
  return cpy;
}

// Returns the new main env.
env* ag_lex_push(env* main, env* other) {
  env* it;
  for (it = other; it && it->next; it = it->next)
    { assert(!it->global); }
  it->next = main;
  return other;
}

env* ag_lex_boundary(env* e) {
  env* it;
  for (it = other; it && !it->boundary; it = it->next)
    { ; }
  return it;
}

// Returns the new main env.
env* ag_lex_pop(env* main) {
  env* boundary = ag_lex_boundary(main);
  if (!boundary || !boundary->next) {
    return NULL;
  } else {
    env* result = boundary->next;
    boundary->next = NULL;
    return result;
  }
}

env* ag_lex_cp(env* e) {
  env* result = mk_env();
  env* rit = result;
  env* it;
  env* cpy;
  for (it = e; it; it = it->next) {
    cpy = ag_lex_cp_top(it);
    rit->next = cpy;
    rit = rit->next;
  }
  if (result->next) {
    return result->next;
  } else {
    result;
  }
}

env* ag_lex_cp_top(env* e) {
  env* result = mk_env();
  bind* it;
  for (it = *e->bhash; it; it=it->hh.next) {
    ag_lex_add(result, ag_lex_cp_bind(b));
  }
  return result;
}

void test_ag_lex_add__single_add() {
  ag_val* s1 = mk_ag_val_symbol(strdup("derp"));
  ag_val* v = mk_ag_val_long(42);
  bind* b = mk_bind(v, s1, false);
  env* e = mk_env();
  
  ag_lex_add(e, b);
  bind* found = ag_lex_find(e, s1);
  assert(found);
  assert(found == b);
  assert(found->val->val.Long == 42);
  
  del_env(e);
  del_ag_val(v);
  del_ag_val(s1);
  return;
}

void test_ag_lex_add__multi_add() {
  eputln("Begin.");
  ag_val* s1 = mk_ag_val_symbol(strdup("derp"));
  ag_val* s2 = mk_ag_val_symbol(strdup("hello"));
  ag_val* s3 = mk_ag_val_symbol(strdup("dorp"));
  ag_val* v1 = mk_ag_val_long(42);
  ag_val* v2 = mk_ag_val_symbol(strdup("ohai"));
  ag_val* vl1 = mk_ag_val_symbol(strdup("I'm in"));
  ag_val* vl2 = mk_ag_val_symbol(strdup("a list!"));
  ag_val* v3 = mk_ag_val_list(mk_val_list(vl1, vl2));
  bind* b1 = mk_bind(v1, s1, false);
  bind* b2 = mk_bind(v2, s2, false);
  bind* b3 = mk_bind(v3, s3, false);
  env* e = mk_env();

  ag_lex_add(e, b1);
  ag_lex_add(e, b2);
  ag_lex_add(e, b3);
  bind* found1 = ag_lex_find(e, s1);
  bind* found2 = ag_lex_find(e, s2);
  bind* found3 = ag_lex_find(e, s3);
  assert(found1);
  assert(found2);
  assert(found3);
  assert(found1->val->val.Long == 42);
  assert(!strcmp(found2->val->val.Symbol, "ohai"));
  assert(found3->val->type == AG_TYPE_LIST);
  assert(!strcmp(found3->val->val.List->head->val.Symbol, "I'm in"));
  assert(!strcmp(found3->val->val.List->tail->val.Symbol, "a list!"));
  
  del_env(e);
  del_ag_val(v1);
  del_ag_val(v2);
  del_ag_val(v3);
  del_ag_val(s1);
  del_ag_val(s2);
  del_ag_val(s3);
  eputln("Pass.");
}

void test_ag_lex_push__multi_push() {
  eputln("Begin.");
  ag_val* s = mk_ag_val_symbol(mk_val_symbol(strdup("derp")));
  ag_val* v1 = mk_ag_val_long(42);
  ag_val* v2 = mk_ag_val_symbol(mk_val_symbol(strdup("ohai")));
  ag_val* vl1 = mk_ag_val_symbol(mk_val_symbol(strdup("I'm in")));
  ag_val* vl2 = mk_ag_val_symbol(mk_val_symbol(strdup("a list!")));
  ag_val* v3 = mk_ag_val_list(mk_val_list(vl1, vl2));
  bind* b1 = mk_bind(v1, s, false);
  bind* b2 = mk_bind(v2, s, false);
  bind* b3 = mk_bind(v3, s, false);
  env* e = mk_env();

  ag_lex_push(e, b1);
  ag_lex_push(e, b2);
  ag_lex_push(e, b3);
  bind* found = ag_lex_find(e, s);
  assert(found);
  assert(found->next);
  assert(found->next->next);
  assert(!found->next->next->next);
  assert(found->val == v3);
  assert(found->next->val == v2);
  assert(found->next->next->val == v1);
  assert(found->val->val.List->head == vl1);
  assert(found->val->val.List->tail == vl2);
  assert(!strcmp(found->val->val.List->head->val.Symbol, "I'm in"));
  assert(found->next->next->val->val.Long == 42);

  del_env(e);
  del_ag_val(s);
  del_ag_val(v1);
  del_ag_val(v2);
  del_ag_val(v3);
  del_ag_val(vl1);
  del_ag_val(vl2);
  eputln("Pass.");
}

void test_variable() {
  eputln("Begin.");
  test_ag_lex_add__single_add();
  test_ag_lex_add__multi_add();
  test_ag_lex_push__multi_push();
  eputln("Pass.");
}
