#pragma once

#include "lval.h"

static const lval_err DIV_ZERO_ERR = {.msg = "ERROR: Division by zero."};
static const lval_err SYMBOL_UNDEFINED_ERR = {.msg = "ERROR: Symbol not defined."};
static const lval_err FEATURE_INCOMPLETE_ERR = {.msg = "ERROR: Feature unavailable."};
static const lval_err UNREACHABLE_ERR = {.msg = "ERROR: The program should never run this code."
					 "  This is really bad."};
static const lval_err OUT_OF_MEMORY_ERR = {.msg = "ERROR: Out of memory."};
