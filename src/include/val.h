#pragma once

#include "string-helpers.h"
#include "types.h"

// Return a val_value representing the long x.
ag_val* make_ag_val();
void ag_val_del(ag_val* v);

ag_val* make_ag_val_long(val_long x);
ag_val* make_ag_val_err(val_err e);
ag_val* make_ag_val_bool(val_bool b);
ag_val* make_ag_val_list(val_list* list);
ag_val* make_ag_val_symbol(val_symbol sym);
ag_val* ag_pop(ag_val* v);
