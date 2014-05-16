#pragma once

#include "types.h"

#include "mpc.h"

ag_val* ag_read(mpc_ast_t* ast);
ag_val* ag_read_long(mpc_ast_t* ast);
ag_val* ag_read_nil(mpc_ast_t* ast);
ag_val* ag_read_symbol(mpc_ast_t* ast);
ag_val* ag_read_list(mpc_ast_t* ast);

