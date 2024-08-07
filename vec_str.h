#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec.h"
#include <stdbool.h>

bool vec_str_eq(void* s, void* t);

void vec_str_destroy(void* value);

// Allocates a new vec of char*'s owned by the vec and with default capacity
// of 8.
#define vec_str_alloc(...)                                                 \
    vec_alloc_((vec_alloc_args){.cap = 8,                                  \
                                .eq = vec_str_eq,                          \
                                .destroy = vec_str_destroy,                \
                                __VA_ARGS__})

// Returns the vec's char* value at position index.
// vec retains ownership, so do not delete the value.
#define vec_str_get(v, index) (char*)vec_get((v), (index))

// Sets the vec's char* value at position index to the given value and
// returns the old char* value from that position.
// vec takes ownership of the new value (e.g., use strdup()).
// The returned char* value is now owned by the caller.
#define vec_str_replace(v, index, value)                                   \
    (char*)vec_replace((v), (index), (value))

// Returns and removes the char* value at the given index and closes up the
// gap.
// The returned char* value is now owned by the caller.
#define vec_str_take(v, index) (char*)vec_take((v), (index))

// Removes and returns the last char* value. Only use if v.isempty() is
// false.
// The returned char* value is now owned by the caller.
#define vec_str_pop(v) (char*)vec_pop((v))

// Returns a deep copy of the vec including eq and destroy, copying each
// char* using strdup().
vec vec_str_copy(vec* v);
