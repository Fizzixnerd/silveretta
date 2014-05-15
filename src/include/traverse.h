#pragma once

#include "mpc.h"

void ag_ast_traverse(mpc_ast_t* ast, void (*func)(mpc_ast_t*, void*), void* func_state);

