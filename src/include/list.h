#pragma once

#include "types.h"

val_list* make_val_list(ag_val* head, ag_val* tail);
void val_list_del(val_list* list);
ag_val* ag_list_append(ag_val* v, ag_val* tail);
ag_val* val_list_pop(val_list* v);
