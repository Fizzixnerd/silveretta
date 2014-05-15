#include "debug.h"
#include "mpc.h"

#include <string.h>

void ag_ast_traverse(mpc_ast_t* ast, void (*func)(mpc_ast_t*, void*), void* func_state) {
  (*func)(ast);
  for (int i = 0; i < ast->children_num; ++i) {
    ag_ast_traverse(ast->children[i], func);
  }
}
