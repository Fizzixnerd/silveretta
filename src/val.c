#include "val.h"
#include "macros.h"
#include "string-helpers.h"
#include "debug.h"
#include "errors.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static ag_val* OOM_ERR;

ag_val* oom_err() {
  *OOM_ERR = {.type = AG_TYPE_ERR, .val.Error = OUT_OF_MEMORY_ERROR};
  return OOM_ERR;
}


ag_val* make_ag_val() {
  return malloc(sizeof(ag_val));
}

ag_val* make_ag_val_long(val_long x) {
  ag_val* v = make_ag_val();
  if (v) {
    *v = {.type = AG_TYPE_LONG, .val.Long = x};
    return v;
  } else {
    return oom_err();
  }
}

ag_val* make_ag_val_err(val_err e) {
  ag_val* v = make_ag_val();
  if (v) {
    *v = {.type = AG_TYPE_ERR, .val.Error = e};
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
    *v = {.type = AG_TYPE_BOOL, .val.Bool = b};
    return v;
  } else {
    return oom_err();
  }
}

ag_val* make_ag_val_list(val_list* list) {
  ag_val* v = make_ag_val();
  if (v) {
    *v = {.type = AG_TYPE_LIST, .val.List = list};
    return v;
  } else {
    return oom_err();
  }
}

ag_val* make_ag_val_symbol(val_symbol sym) {
  ag_val* v = make_ag_val();
  if (v) {
    *v = {.type = AG_TYPE_SYMBOL, .val.Symbol = sym};
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
    ag_val_del(v->val->List);
    break;
  }
  free(v);
  return;
}

val_list* make_val_list(ag_val* head, val_list* tail) {
  
  val_list list = malloc(sizeof(val_list));
  if (list) {
    *list = {.head = head, .tail = tail};
  } else {
    printf("%s", "Out of memory!");
    abort();
  }
  return list;
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

ag_val* ag_read_nil(mpc_ast* ast) {
  assert(strstr(ast->tag, "nil"));
  return NULL;
}

ag_val* ag_read_symbol(mpc_ast* ast) {
  assert(strstr(ast->tag, "symbol"));
  return make_ag_val_symbol(strdup(ast->contents));
}

ag_val* ag_read_list(mpc_ast* ast) {
  assert(strstr(ast->tag, "list"));
  val_list* list = make_val_list(ag_read(ast->children->contents[1]), NULL);

  int i = 2;
  val_list* curr_list = NULL;
  val_list* next_list = list;
  // while not ")"
  while (strcmp(ast->children->contents[i], ")") && (i < ast->children_num)) {
    if (!strcmp(ast->children[i], "regex") continue;
    curr_list = next_list;
    next_list = make_val_list(ag_read(ast->children[i]->contents), NULL)
    val_list_append_list(curr_list, next_list);
    ++i;
  }
  return make_ag_val_list(list);
}

ag_val ag_read(mpc_ast* ast) {

  if (strstr(ast->tag, "long_num")) return ag_read_long(ast);
  if (strstr(ast->tag, "symbol")) return ag_read_symbol(ast);
  if (strstr(ast->tag, "nil")) return ag_read_nil(ast);
  if (!strcmp(ast->tag, ">") || strstr(ast->tag, "list")) {
    return ag_read_list(ast);
  }
}
   
char* ag_val_to_string(ag_val* v) {
  // FIXME : Finish this.
  // The caller must free the string.
  if (!v) {
    return strdup("nil");
  }
  switch (v.type) {
  case AG_TYPE_LONG:
    return ltoa(v.val.Long);
    break;
  case AG_TYPE_ERR:
    return astrcpy(v.val.Error.msg);
    break;
  case AG_TYPE_BOOL:
    return v.val.Bool ? strdup("true") : strdup("false");
    break;
  case AG_TYPE_LIST:
    const ag_val head = v.val.List.head;
    const ag_val tail = make_ag_val_list(v.val.List.tail);
    char* head_str = val_to_string(head);
    char* tail_str = tail.val.List ? val_to_string(tail) : NULL;
    int n;
    if (tail_str) {
      n = snprintf(NULL, 0, "(%s %s)", head_str, tail_str);
      char* buf = malloc(sizeof(char) * (n+1));
      snprintf(buf, n+1, "(%s %s)", head_str, tail_str);
      free(head_str);
      free(tail_str);
      return buf;
    } else {
      n = snprintf(NULL, 0, "(%s)", head_str);
      char* buf = malloc(sizeof(char) * (n+1));
      snprintf(buf, n+1, "(%s)", head_str);
      free(head_str);
      return buf;
    }
    break;
  case AG_TYPE_NIL:
    return strdup("nil");
    break;
  default:
    #include "stringify.h"
    eprintf("Error: val_to_string doesn't know how to stringify type %s.", STRINGIFY(v.type));
    #include "stringify-cleanup.h"
    return "Error: Can't stringify the lval given to lval_to_string.";
  }
}

