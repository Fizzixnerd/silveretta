#pragma once

#include "types.h"

struct str_list {
  char* str;
  struct str_list* next;
};
typedef struct str_list str_list;

str_list* val_list_alprint(val_list* v);
void str_list_del(struct str_list* sl);
