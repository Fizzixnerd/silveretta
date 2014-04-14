#define _WDEBUG

#include "debug.h"
#include "mpc.h"

#include <string.h>

void wl_ast_traverse(mpc_ast_t* ast, void (*func)(mpc_ast_t*, void*), void* func_state) {
  (*func)(ast);
  for (int i = 0; i < ast->children_num; ++i) {
    wl_ast_traverse(ast->children[i], func);
  }
}
