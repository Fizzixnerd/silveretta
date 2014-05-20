#include "print.h"
#include "debug.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define oom_asprintf {do { eprintf("%s", "out of memory!"); abort(); } while(0);}

char* ag_asprint(ag_val* v) {
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
    ;
    // FIXME WTF WHY DO I NEED THIS RANDOMLY?

    if (!v->val.List) {
      return strdup("()");
    }

    string result;
    size_t result_size;
    FILE* fp = open_memstream(&result, &result_size);
    fprintf(fp, "%s", "(");
    for (val_list* vl = v->val.List; vl; vl = vl->tail->val.List) {
      fprintf(fp, "%s ", ag_asprint(vl->head));
      if (vl->tail->type != AG_TYPE_LIST) {
	fprintf(fp, "%s ", ag_asprint(vl->tail));
	break;
      }
    }
    fclose(fp);
    result[result_size-1] = ')';
    return result;
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

#undef oom_printf

void ag_print(ag_val* value) {
  char* value_str = ag_asprint(value);
  printf("%s\n", value_str);
  free(value_str);
}
