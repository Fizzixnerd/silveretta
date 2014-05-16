#include "string-helpers.h"

#include <stdlib.h>

void str_list_del(struct str_list* sl) {
  if (sl) {
    if (sl->next->str) {
      str_list_del(sl->next);
    }
    free(sl->str);
    free(sl);
  }
}

