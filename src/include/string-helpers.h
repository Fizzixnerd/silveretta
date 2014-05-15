#pragma once

#include "val.h"

typedef char* string;
typedef char** string_array;

typedef struct str_list {
  char* str;
  struct str_list* next;
} str_list;

