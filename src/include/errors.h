#pragma once

#include "val.h"

#include <string.h>

static const val_err DIV_ZERO_ERR = {.msg = strdup("ERROR: Division by zero.")};
static const val_err SYMBOL_UNDEFINED_ERR = {.msg = strdup("ERROR: Symbol not defined.")};
static const val_err FEATURE_INCOMPLETE_ERR = {.msg = strdup("ERROR: Feature unavailable.")};
static const val_err UNREACHABLE_ERR = {.msg = strdup("ERROR: The program should never run this code."
						      "  This is really bad.")};
static const val_err OUT_OF_MEMORY_ERR = {.msg = strdup("ERROR: Out of memory.")};
