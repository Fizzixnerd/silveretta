#pragma once

#include "val.h"

static const val_err DIV_ZERO_ERR = {.msg = "ERROR: Division by zero."};
static const val_err SYMBOL_UNDEFINED_ERR = {.msg = "ERROR: Symbol not defined."};
static const val_err FEATURE_INCOMPLETE_ERR = {.msg = "ERROR: Feature unavailable."};
static const val_err UNREACHABLE_ERR = {.msg = "ERROR: The program should never run this code."
					 "  This is really bad."};
static const val_err OUT_OF_MEMORY_ERR = {.msg = "ERROR: Out of memory."};
