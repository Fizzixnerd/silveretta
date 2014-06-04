#pragma once

#include "types.h"

val_list* mk_val_list(ag_val* head, ag_val* tail);
void del_val_list(val_list* list);
ag_val* ag_list_append(ag_val* v, ag_val* tail);
ag_val* val_list_pop(val_list* v);
ag_val* ag_head(ag_val* list);
ag_val* ag_tail(ag_val* list);
