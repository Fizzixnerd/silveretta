#include "list.h"
#include "val.h"
#include "debug.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

val_list* mk_val_list(ag_val* head, ag_val* tail) {
  
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

void del_val_list(val_list* list) {
  free(list);
}

ag_val* ag_list_append(ag_val* v, ag_val* tail) {
  if (!v) {
    if (!tail) {
      // both nil.
      return NULL;
    } else {
      // tail non-nil
      if (tail->type == AG_TYPE_LIST) {
	return tail;
      } else {
	return mk_ag_val_list(mk_val_list(tail, NULL));
      }
    }
  } else {
    // v is non-nil list
    assert(v->type == AG_TYPE_LIST);
    val_list* vl = v->val.List;
    if (!vl->tail) {
      // v's tail is nil
      vl->tail = tail;
    } else {
      // v's tail is non-nil
      ag_list_append(vl->tail, tail);
    }
  }
  return v;
}

ag_val* val_list_pop(val_list* v) {
  return v ? v->head : NULL;
}

ag_val* ag_head(ag_val* list) {
  assert(list->type == AG_TYPE_LIST);
  return list->val.List->head;
}

ag_val* ag_tail(ag_val* list) {
  assert(list->type == AG_TYPE_LIST);
  return list->val.List->tail;
}
