#pragma once

#include "types.h"

val_list* make_val_list(ag_val* head, val_list* tail);
void val_list_del(val_list* list);
val_list* val_list_append_list(val_list* v, val_list* tail);
val_list* val_list_append_ag_val(val_list* v, ag_val* tail_val);
