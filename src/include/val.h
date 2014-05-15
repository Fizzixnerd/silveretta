#pragma once

#include "macros.h"



// val stands for "lisp value".
// This is basically an implementation of a tagged union, at the end of the day.

/* Enum which defines the valid types for ag_vals.  AG_TYPE_ERR is
   first, to allow the idiom:

   ag_val val;
   if ((val = foo()).type) {
     do_normal_stuff(val);
   }
   else {
     handle_error(val);
   } */
typedef enum {
  AG_TYPE_ERR = 0, // Error type

  AG_TYPE_BOOl,
  AG_TYPE_CHAR, // Machine integer types
  AG_TYPE_UCHAR,
  AG_TYPE_INT,
  AG_TYPE_UINT,
  AG_TYPE_LONG,
  AG_TYPE_ULONG,
  AG_TYPE_LLONG,
  AG_TYPE_ULLONG,
  
  AG_TYPE_FLOAT, // Machine floating-point types 
  AG_TYPE_CFLOAT, // Complex floating-point numbers are prefixed with "C"
  AG_TYPE_DOUBLE,
  AG_TYPE_CDOUBLE,
  AG_TYPE_LDOUBLE,
  AG_TYPE_CLDOUBLE,
  AG_TYPE_PTR, // Machine data pointer
  AG_TYPE_FPTR, // Machine function pointer
  AG_TYPE_LOBJ, // Lisp object

  AG_TYPE_LIST,
  AG_TYPE_SYMBOL,
} val_type;

typedef struct {
  char* msg;
} val_err;

typedef bool val_bool
typedef signed char val_char;
typedef unsigned char val_uchar;
typedef int val_int;
typedef unsigned int val_uint;
typedef long val_long;
typedef unsigned long val_ulong;
typedef long long val_llong;
typedef unsigned long long val_ullong;

typedef float val_float;
typedef complex float val_cfloat;
typedef double val_double;
typedef complex double val_cdouble;
typedef long double val_ldouble;
typedef complex long double val_cldouble;

typedef void* val_ptr;
// FIXME: This should actually be a function pointer, but I don't know
// how that works...
typedef void* val_fptr;
typedef struct {
  void* ptr;
  char* type;
} val_obj;

typedef struct {
  ag_val* head;
  val_list* tail;
} val_list;

typedef char* val_symbol;

// TODO: Also need to add the val_fptr and val_obj to this union
typedef union {
  val_err Error;
  val_bool Bool;
  val_char Char;
  val_uchar UChar;
  val_int Int;
  val_uint UInt;
  val_long Long;
  val_ulong ULong;
  val_llong LLong;
  val_ullong ULLong;
  val_float Float;
  val_cfloat CFloat;
  val_double Double;
  val_cdouble CDouble;
  val_ldouble LDouble;
  val_cldouble CLDouble;
  val_ptr Ptr;
  val_obj Obj;
  val_list* List;
  val_symbol Symbol;
} val_value;

// Struct which holds the return value/error conditions of an expression.
typedef struct {
  val_value val;
  val_type type;
} ag_val;

// Return a val_value representing the long x.
ag_val* make_ag_val_long(val_long x);
ag_val* make_ag_val_err(val_err e);
ag_val* make_ag_val_bool(val_bool b);
ag_val* make_ag_val_list(val_list list);
ag_val* make_ag_val_symbol(val_symbol sym);
val_list make_val_list(ag_val* head, val_list* tail);

// You must free the string when you are done.
char* ag_val_to_string(ag_val v);

