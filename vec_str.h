#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

bool vec_str_eq(void* s, void* t) { return strcmp((s), (t)) == 0; }

void vec_str_destroy(void* value) { free((char*)value); }

#define vec_str_alloc(...)                                                 \
    vec_alloc_((vec_alloc_args){                                           \
        .cap = 8, .destroy = vec_str_destroy, __VA_ARGS__})

#define vec_str_get(v, index) (char*)vec_get((v), (index))

#define vec_str_replace(v, index, value)                                   \
    (char*)vec_replace((v), (index), (value))

#define vec_str_take(v, index) (char*)vec_take((v), (index))

#define vec_str_pop(v) (char*)vec_pop((v))
