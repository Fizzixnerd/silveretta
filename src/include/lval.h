#ifndef _SIMPLE_LISP_LVAL_H
#define _SIMPLE_LISP_LVAL_H

/* Enum which defines the valid types for wl_lvals */
typedef enum {
  LTYPE_NUM,
  LTYPE_ERR
} lval_type;

typedef long lval_value;

typedef enum {
  LERR_OK,
  LERR_DIV_ZERO,
  LERR_BAD_OP,
  LERR_BAD_NUM
} lval_error;

/* Struct which holds the return value/error conditions of an expression. */
typedef struct {
  lval_type type;
  lval_value val;
  lval_error err;
} wl_lval;

/* Return an lval_value representing the long x. */
lval_value lval_new_num(long x);
wl_lval lval_num(long x);
wl_lval lval_err(lval_error e);
/* You must free the string when you are done. */
char* lval_to_string(wl_lval v);

#endif /* _SIMPLE_LISL_LVAL_H */
