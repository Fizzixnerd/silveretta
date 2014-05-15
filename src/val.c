#define _GNU_SOURCE

#include "val.h"
#include "macros.h"
#include "string-helpers.h"
#include "debug.h"
#include "errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static ag_val* OOM_ERR;

static ag_val* oom_err() {
  OOM_ERR->type = AG_TYPE_ERR;
  OOM_ERR->val.Error = make_val_err(OUT_OF_MEMORY_ERR);
  return OOM_ERR;
}


ag_val* make_ag_val() {
  return malloc(sizeof(ag_val));
}

ag_val* make_ag_val_long(val_long x) {
  ag_val* v = make_ag_val();
  if (v) {
    v->type = AG_TYPE_LONG;
    v->val.Long = x;
    return v;
  } else {
    return oom_err();
  }
}

ag_val* make_ag_val_err(val_err e) {
  ag_val* v = make_ag_val();
  if (v) {
    v->type = AG_TYPE_ERR;
    v->val.Error = e;
    return v;
  } else {
    return oom_err();
  }
}

// val_err gets ownership of the msg
val_err make_val_err(char* msg) {
  val_err e = {.msg = msg};
  return e;
}

ag_val* make_ag_val_bool(val_bool b) {
  ag_val* v = make_ag_val();
  if (v) {
    v->type = AG_TYPE_BOOL;
    v->val.Bool = b;
    return v;
  } else {
    return oom_err();
  }
}

ag_val* make_ag_val_list(val_list* list) {
  ag_val* v = make_ag_val();
  if (v) {
    v->type = AG_TYPE_LIST;
    v->val.List = list;
    return v;
  } else {
    return oom_err();
  }
}

ag_val* make_ag_val_symbol(val_symbol sym) {
  ag_val* v = make_ag_val();
  if (v) {
    v->type = AG_TYPE_SYMBOL;
    v->val.Symbol = sym;
    return v;
  } else {
    return oom_err();
  }
}

void ag_val_del(ag_val *v) {
  if (!v) return;
  
  switch (v->type) {
  case AG_TYPE_LONG:
  case AG_TYPE_BOOL:
    break;
  case AG_TYPE_ERR:
    free(v->val.Error.msg);
    break;
  case AG_TYPE_SYMBOL:
    free(v->val.Symbol);
    break;
  case AG_TYPE_LIST:
    val_list_del(v->val.List);
    break;
  }
  free(v);
  return;
}

val_list* make_val_list(ag_val* head, val_list* tail) {
  
  val_list* list = malloc(sizeof(val_list));
  if (list) {
    list->head = head;
    list->tail = tail;
    return list;
  } else {
    printf("%s", "Out of memory!");
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
	     
ag_val* ag_read_long(mpc_ast_t* ast) {
  assert(strstr(ast->tag, "long_num"));
  errno = 0;
  long x = strtol(ast->contents, NULL, 10);
  return errno != ERANGE ? make_ag_val_long(x) :
                           make_ag_val_err(make_val_err("Invalid long."));
}

ag_val* ag_read_nil(mpc_ast_t* ast) {
  assert(strstr(ast->tag, "nil"));
  return NULL;
}

ag_val* ag_read_symbol(mpc_ast_t* ast) {
  assert(strstr(ast->tag, "symbol"));
  return make_ag_val_symbol(strdup(ast->contents));
}

ag_val* ag_read_list(mpc_ast_t* ast) {
  assert(strstr(ast->tag, "list") || !strcmp(ast->tag, ">"));
  val_list* list = make_val_list(ag_read(ast->children[1]), NULL);

  int i = 2;
  val_list* curr_list = NULL;
  val_list* next_list = list;
  // while not ")"
  while (strcmp(ast->children[i]->contents, ")") && (i < ast->children_num)) {
    if (!strcmp(ast->children[i]->contents, "regex")) continue;
    curr_list = next_list;
    next_list = make_val_list(ag_read(ast->children[i]), NULL);
    val_list_append_list(curr_list, next_list);
    ++i;
  }
  return make_ag_val_list(list);
}

ag_val* ag_read(mpc_ast_t* ast) {
  
  if (strstr(ast->tag, "long_num")) return ag_read_long(ast);
  if (strstr(ast->tag, "symbol")) return ag_read_symbol(ast);
  if (strstr(ast->tag, "nil")) return ag_read_nil(ast);
  if (!strcmp(ast->tag, ">")) {
    return ag_read(ast->children[1]);
  }
  if (strstr(ast->tag, "list")) {
    return ag_read_list(ast);
  }
  return make_ag_val_err(make_val_err(strdup("Read error.")));
}

struct str_list* val_list_alprint(val_list* v) {
  // a == allocated
  // l == list
  // Must free.
  struct str_list* head = malloc(sizeof(str_list));
  struct str_list* sl_it = head;
  val_list* v_it = v;

  do {
    sl_it->str = ag_sprint(v->head);
    sl_it->next = malloc(sizeof(str_list));
    sl_it->next->str = NULL;
    sl_it->next->next = NULL;
    sl_it = sl_it->next;
    v_it = v_it->tail;
  }
  while (v_it);
  return head;
}

void str_list_del(struct str_list* sl) {
  if (sl) {
    if (sl->next->str) {
      str_list_del(sl->next);
    }
    free(sl->str);
    free(sl);
  }
}

#define oom_asprintf {do { eprintf("%s", "out of memory!"); abort(); } while(0);}

char* ag_sprint(ag_val* v) {
  // FIXME : Finish this.
  // The caller must free the string.
  if (!v) {
    return strdup("nil");
  }
  switch (v->type) {
  case AG_TYPE_LONG:
    ;
    char* strp;
    if (asprintf(&strp, "%li", v->val.Long) == -1) {
      oom_asprintf;
    }
    return strp;
    break;
  case AG_TYPE_ERR:
    return strdup(v->val.Error.msg);
    break;
  case AG_TYPE_BOOL:
    return v->val.Bool ? strdup("true") : strdup("false");
    break;
  case AG_TYPE_LIST:
    ; // FIXME WTF WHY DO I NEED THIS RANDOMLY?
    string curr;
    string next;
    str_list* sl = val_list_alprint(v->val.List);
    str_list* sl_it = sl;
    if (asprintf(&next, "(%s", sl->str) == -1) {
      oom_asprintf;
    }
    while (sl_it->next->str) {
      curr = next;
      if (asprintf(&next, "%s %s", curr, sl_it->next->str) == -1) {
	oom_asprintf;
      }
      free(curr);
      sl_it = sl_it->next;
    }
    str_list_del(sl);
    curr = next;
    if (asprintf(&next, "%s)", curr) == -1) {
      oom_asprintf;
    }
    free(curr);
    return next;
    break;
  case AG_TYPE_SYMBOL:
    return strdup(v->val.Symbol);
    break;
  default:
    #include "stringify.h"
    eprintf("Error: val_to_string doesn't know how to stringify type %s.", STRINGIFY(v.type));
    #include "stringify-cleanup.h"
    return "Error: Can't stringify the lval given to lval_to_string.";
  }
}

void ag_print(ag_val* value) {
  char* value_str = ag_sprint(value);
  printf("%s", value_str);
  free(value_str);
}
