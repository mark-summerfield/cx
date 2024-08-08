// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str.h"
#include <stdlib.h>
#include <string.h>

bool vec_str_eq(void* s, void* t) { return strcmp((s), (t)) == 0; }

void vec_str_destroy(void* value) { free((char*)value); }

void* vec_str_cp(void* value) { return (void*)strdup((char*)value); }
