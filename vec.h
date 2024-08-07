#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <stdbool.h>
#include <stddef.h>

typedef struct vec {
    size_t size; // This is "end", i.e., one past the last value
    size_t cap;  // The size of the allocated array
    void** values;
    bool (*eq)(void* a, void* b);
    void (*destroy)(void* values);
} vec;

typedef struct {
    size_t cap;
    bool (*eq)(void* a, void* b);
    void (*destroy)(void* values);
} vec_alloc_args;

// Allocates a new vec with default capacity of 8. Set the initial
// capacity with .cap. Caller must supply an eq function (for find) and
// a destroy function if values are pointers *and* owned by the vec
// (e.g., owned char*; but the default of NULL is fine for say, int or
// unowned char*).
#define vec_alloc(...)                                                     \
    vec_alloc_((vec_alloc_args){.cap = 8, .destroy = NULL, __VA_ARGS__})
vec vec_alloc_(vec_alloc_args args);

// Destroys the vec freeing its memory and if .destroy is not NULL, also
// freeing every value. The vec is not usable after this.
void vec_free(vec* v);

// Clears the vec freeing its memory and if .destroy is not NULL, also
// freeing every value. The vec is usable after this (e.g., push() etc).
void vec_clear(vec* v);

// Returns whether the vec is empty.
#define vec_isempty(v) ((v)->size > 0)

// Returns the vec's size.
#define vec_size(v) ((v)->size)

// Returns the vec's capacity.
#define vec_cap(v) ((v)->cap)

// Returns the vec's str at position index.
const void* vec_get(vec* v, size_t index);

// Sets the vec's str at position index to the given value.
// If .destroy is not NULL, frees the old string.
void vec_set(vec* v, size_t index, void* value);

// Sets the vec's str at position index to the given value and
// returns the old str from that position. Caller now owns the old str.
void* vec_replace(vec* v, size_t index, void* value);

// Removes the value at the given index and closes up the gap.
// If .destroy is not NULL, frees the removed string.
void vec_remove(vec* v, size_t index);

// Returns and removes the value at the given index and closes up the
// gap. The returned str is now owned by the caller.
void* vec_take(vec* v, size_t index);

// Removes and returns the last value. Only use if v.isempty() is false.
// Ownership passes to the caller.
void* vec_pop(vec* v);

// Pushes a new value onto the end of the vec, increasing the vec's size
// (and cap) if necessary.
void vec_push(vec* v, void* value);

typedef struct {
    size_t index;
    bool found;
} vec_find_result;

// Returns the index of value in the vec and true or 0 and false.
vec_find_result vec_find(vec* v, void* value);

// Returns a deep copy of the vec including .eq and .destroy using the
// given cp function for copying each value.
vec vec_copy(vec* v, void* (*cp)(void*));

// To iterate:
//  for (size_t i = 0; i < v.size(); i++) void* value = v.get(i);
