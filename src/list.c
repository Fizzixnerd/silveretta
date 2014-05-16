#include "list.h"
#include "val.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

val_list* make_val_list(ag_val* head, val_list* tail) {
  
  val_list* list = malloc(sizeof(val_list));
  if (list) {
    list->head = head;
    list->tail = tail;
    return list;
  } else {
    eprintf("%s", "Out of memory!");
    abort();
  }
}

void val_list_del(val_list* list) {
  ag_val_del(list->head);
  val_list_del(list->tail);
  free(list);
}

val_list* val_list_append_list(val_list* v, val_list* tail) {
  if (!v->tail) {
    v->tail = tail;
  } else {
    val_list_append_list(v->tail, tail);
  }
  return v;
}

val_list* val_list_append_ag_val(val_list* v, ag_val* tail_val) {
  val_list* tail = make_val_list(tail_val, NULL);
  return val_list_append_list(v, tail);
}

