#ifndef _SIMPLE_LISP_LVAL_H
#define _SIMPLE_LISP_LVAL_H

#include "macros.h"

// lval stands for "lisp value".
// This is basically an implementation of a tagged union, at the end of the day.

/* Enum which defines the valid types for wl_lvals.  LTYPE_ERR is
   first, to allow the idiom:

   wl_lval lval;
   if ((lval = foo()).type) {
     do_normal_stuff(lval);
   }
   else {
     handle_error(lval);
   } */
typedef enum {
  LTYPE_ERR = 0, // Error type
  
  LTYPE_CHAR, // Machine integer types
  LTYPE_UCHAR,
  LTYPE_INT,
  LTYPE_UINT,
  LTYPE_LONG,
  LTYPE_ULONG,
  LTYPE_LLONG,
  LTYPE_ULLONG,
  
  LTYPE_FLOAT, // Machine floating-point types 
  LTYPE_CFLOAT, // Complex floating-point numbers are prefixed with "C"
  LTYPE_DOUBLE,
  LTYPE_CDOUBLE,
  LTYPE_LDOUBLE,
  LTYPE_CLDOUBLE,
  LTYPE_PTR, // Machine data pointer
  LTYPE_FPTR, // Machine function pointer
  LTYPE_LOBJ // Lisp object
} lval_type;

typedef struct {
  char* msg;
} lval_err;

typedef signed char lval_char;
typedef unsigned char lval_uchar;
typedef int lval_int;
typedef unsigned int lval_uint;
typedef long lval_long;
typedef unsigned long lval_ulong;
typedef long long lval_llong;
typedef unsigned long long lval_ullong;

typedef float lval_float;
typedef complex float lval_cfloat;
typedef double lval_double;
typedef complex double lval_cdouble;
typedef long double lval_ldouble;
typedef complex long double lval_cldouble;

typedef void* lval_ptr;
// FIXME: This should actually be a function pointer, but I don't know
// how that works...
typedef void* lval_fptr;
typedef struct {
  void* ptr;
  char* type;
} lval_obj;

// TODO: Also need to add the lval_fptr and lval_obj to this union
typedef union {
  lval_err Error;
  lval_char Char;
  lval_uchar UChar;
  lval_int Int;
  lval_uint UInt;
  lval_long Long;
  lval_ulong ULong;
  lval_llong LLong;
  lval_ullong ULLong;
  lval_float Float;
  lval_cfloat CFloat;
  lval_double Double;
  lval_cdouble CDouble;
  lval_ldouble LDouble;
  lval_cldouble CLDouble;
  lval_ptr Ptr;
  lval_obj Obj;
} lval_value;

// Struct which holds the return value/error conditions of an expression.
typedef struct {
  lval_value val;
  lval_type type;
} wl_lval;

// Return an lval_value representing the long x.
wl_lval make_wl_lval_long(lval_long x);
wl_lval make_wl_lval_err(lval_err e);
// You must free the string when you are done.
char* lval_to_string(wl_lval v);

#endif /* _SIMPLE_LISP_LVAL_H */
