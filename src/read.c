#include "read.h"

#include "types.h"
#include "val.h"
#include "list.h"
#include "symbol.h"
#include "error.h"

#include "mpc.h"

#include <string.h>
#include <assert.h>


ag_val* ag_read(mpc_ast_t* ast) {
  
  if (strstr(ast->tag, "long_num")) return ag_read_long(ast);
  if (strstr(ast->tag, "symbol")) return ag_read_symbol(ast);
  if (!strcmp(ast->tag, ">")) return ag_read(ast->children[1]);
  if (strstr(ast->tag, "list")) return ag_read_list(ast);
  
  return mk_ag_val_err(mk_val_err(strdup("Read error.")));
}

ag_val* ag_read_long(mpc_ast_t* ast) {
  assert(strstr(ast->tag, "long_num"));
  errno = 0;
  long x = strtol(ast->contents, NULL, 10);
  return errno != ERANGE ? mk_ag_val_long(x) :
    mk_ag_val_err(mk_val_err(strdup("Invalid long.")));
}

ag_val* ag_read_symbol(mpc_ast_t* ast) {
  assert(strstr(ast->tag, "symbol"));
  return mk_ag_val_symbol(mk_val_symbol(strdup(ast->contents)));
}

ag_val* ag_read_list(mpc_ast_t* ast) {
  assert(strstr(ast->tag, "list") || !strcmp(ast->tag, ">"));
  if (!strcmp(ast->children[1]->contents, ")")) {
    return NULL;
  }

  ag_val* list = mk_ag_val_list(mk_val_list(ag_read(ast->children[1]), NULL));
  int i = 2;
  ag_val* curr_list = NULL;
  ag_val* next_list = list;
  // while not ")"
  while (strcmp(ast->children[i]->contents, ")") && (i < ast->children_num)) {
    if (!strcmp(ast->children[i]->contents, "regex")) continue;
    curr_list = next_list;
    next_list = mk_ag_val_list(mk_val_list(ag_read(ast->children[i]), NULL));
    ag_list_append(curr_list, next_list);
    ++i;
  }
  return list;
}
