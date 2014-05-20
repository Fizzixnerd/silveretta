#pragma once

#include "types.h"
#include "uthash.h"

struct bind {
  val_symbol sym;
  ag_val* val;
  bool dyn;
  struct bind* next;
  UT_hash_handle hh;
};
typedef struct bind bind;
typedef bind* env;

bind* mk_bind(ag_val* v, ag_val* sym, bool dyn);
env* mk_env();
void del_bind(bind* b);
void del_env(env* e);
void ag_lex_add(env* e, bind* b);
bind* ag_lex_replace(env* e, bind* b);
bind* ag_lex_rm(env* e, bind* b);
bind* ag_lex_find(env* e, ag_val* sym);
bind* ag_lex_find_rm(env* e, ag_val* sym);
void ag_lex_push(env* e, bind* b);
bind* ag_lex_pop(env* e, bind* b);
bind* ag_lex_find_pop(env* e, ag_val* sym);
bind* ag_lex_cp(bind* b);
env* ag_lex_push_env(env* main, env* other);
env* ag_lex_pop_env(env* main, env* other);
env* ag_lex_cp_env(env* main);
void test_ag_lex_add__single_add();
void test_ag_lex_add__multi_add();
void test_ag_lex_push__multi_push();
void test_variable();
