#pragma once

#define complex _Complex
#define bool _Bool
typedef char* string;

typedef string ag_symbol;

// ag_val stands for "silveretta value".
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

  AG_TYPE_BOOL,
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
  string msg;
} val_err;

typedef bool val_bool;
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
  string type;
} val_obj;

typedef char* val_symbol;

struct val_list {
  struct ag_val* head;
  struct ag_val* tail;
};
typedef struct val_list val_list;

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
  val_fptr Fptr;
  val_obj Obj;
  val_list* List;
  val_symbol Symbol;
} val_value;

// Struct which holds the return value/error conditions of an expression.
struct ag_val {
  val_value val;
  val_type type;
};
typedef struct ag_val ag_val;

