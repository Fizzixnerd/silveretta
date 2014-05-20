#pragma once

#include "val.h"
#include "types.h"

#include <string.h>

val_err mk_val_err(char* msg);
void del_val_err(val_err e);

#define OUT_OF_MEMORY_ERR strdup("ERROR: Out of memory.")
#define DIV_ZERO_ERR strdup("ERROR: Division by zero.")
static const val_err SYMBOL_UNDEFINED_ERR = {.msg = "ERROR: Symbol not defined."};
static const val_err FEATURE_INCOMPLETE_ERR = {.msg = "ERROR: Feature unavailable."};
static const val_err UNREACHABLE_ERR = {.msg = "ERROR: The program should never run this code."
						      "  This is really bad."};



