#include "read.h"

#include "types.h"
#include "val.h"
#include "list.h"
#include "error.h"

#include "mpc.h"

#include <string.h>
#include <assert.h>


ag_val* ag_read(mpc_ast_t* ast) {
  
  if (strstr(ast->tag, "long_num")) return ag_read_long(ast);
  if (strstr(ast->tag, "symbol")) return ag_read_symbol(ast);
  if (strstr(ast->tag, "nil")) return ag_read_nil(ast);
  if (!strcmp(ast->tag, ">")) {
    return ag_read(ast->children[1]);
  }
  if (strstr(ast->tag, "list")) {
    return ag_read_list(ast);
  }
  return make_ag_val_err(make_val_err(strdup("Read error.")));
}

ag_val* ag_read_long(mpc_ast_t* ast) {
  assert(strstr(ast->tag, "long_num"));
  errno = 0;
  long x = strtol(ast->contents, NULL, 10);
  return errno != ERANGE ? make_ag_val_long(x) :
    make_ag_val_err(make_val_err("Invalid long."));
}

ag_val* ag_read_nil(mpc_ast_t* ast) {
  assert(strstr(ast->tag, "nil"));
  return NULL;
}

ag_val* ag_read_symbol(mpc_ast_t* ast) {
  assert(strstr(ast->tag, "symbol"));
  return make_ag_val_symbol(strdup(ast->contents));
}

ag_val* ag_read_list(mpc_ast_t* ast) {
  assert(strstr(ast->tag, "list") || !strcmp(ast->tag, ">"));
  val_list* list = make_val_list(ag_read(ast->children[1]), NULL);

  int i = 2;
  val_list* curr_list = NULL;
  val_list* next_list = list;
  // while not ")"
  while (strcmp(ast->children[i]->contents, ")") && (i < ast->children_num)) {
    if (!strcmp(ast->children[i]->contents, "regex")) continue;
    curr_list = next_list;
    next_list = make_val_list(ag_read(ast->children[i]), NULL);
    val_list_append_list(curr_list, next_list);
    ++i;
  }
  return make_ag_val_list(list);
}
