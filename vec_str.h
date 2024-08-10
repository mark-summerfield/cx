#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec.h"
#include <stdbool.h>
#include <stdio.h>

bool vec_str_eq(void* s, void* t);

void* vec_str_cp(void* value);

void vec_str_destroy(void* value);

// Allocates a new vec of owned char*'s with default capacity of 32.
#define vec_str_alloc(...)                                                 \
    vec_alloc_((vec_alloc_args){.cap = 32,                                 \
                                .eq = vec_str_eq,                          \
                                .cp = vec_str_cp,                          \
                                .destroy = vec_str_destroy,                \
                                __VA_ARGS__})

// Allocates a new vec of owned char*'s by splitting s on sep; neither
// may be NULL.
vec vec_str_alloc_split(const char* s, const char* sep);

#define vec_str_free() vec_free

// Returns the vec's char* value at position index.
// vec retains ownership, so do not delete the value.
#define vec_str_get(v, index) (char*)vec_get((v), (index))

// Returns the vec's char* value at its last valid index.
// vec retains ownership, so do not delete the value.
const char* vec_str_get_last(const vec* v);

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

// Returns a char* of all the vec's strings joined together with sep. If
// sep is NULL they are joined with no separator.
// The returned char* value is now owned by the caller.
char* vec_str_join(const vec* v, const char* sep);

void vec_str_sort(vec* v);

void vec_str_sort_fold(vec* v);

typedef struct {
    size_t index;
    bool found;
} vec_str_bsearch_result;

vec_str_bsearch_result vec_str_bsearch(const vec* v, const char* s);

// To iterate:
//  for (size_t i = 0; i < vec_size(v); ++i)
//      const char* value = vec_str_get(v, i);
