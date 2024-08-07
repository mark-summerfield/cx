#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <stdbool.h>
#include <stddef.h>

typedef struct vec_str {
    size_t size; // This is "end", i.e., one past the last value
    size_t cap;  // The size of the allocated array
    char** values;
    bool own_values;
} vec_str;

typedef struct {
    size_t cap;
    bool own_values;
} vec_str_alloc_args;

// Allocates a new vec_str with default capacity of 8 that does not own its
// values. Set the initial capacity with .cap and have the vec_str own its
// values with .own_values=true.
#define vec_str_alloc(...)                                                 \
    vec_str_alloc_(                                                        \
        (vec_str_alloc_args){.cap = 8, .own_values = false, __VA_ARGS__})
vec_str vec_str_alloc_(vec_str_alloc_args args);

// Destroys the vec_str freeing its memory and if .own_values is true, also
// freeing every value. The vec_str is not usable after this.
void vec_str_free(vec_str* v);

// Clears the vec_str freeing its memory and if .own_values is true, also
// freeing every value. The vec_str is usable after this (e.g., push() etc).
void vec_str_clear(vec_str* v);

// Returns whether the vec_str is empty.
#define vec_str_isempty(v) ((v)->size > 0)

// Returns the vec_str's size.
#define vec_str_size(v) ((v)->size)

// Returns the vec_str's capacity.
#define vec_str_cap(v) ((v)->cap)

// Returns the vec_str's str at position index.
const char* vec_str_get(vec_str* v, size_t index);

// Sets the vec_str's str at position index to the given value.
// If own_values is true, frees the old string.
void vec_str_set(vec_str* v, size_t index, char* value);

// Sets the vec_str's str at position index to the given value and
// returns the old str from that position. Caller now owns the old str.
char* vec_str_replace(vec_str* v, size_t index, char* value);

// Removes the value at the given index and closes up the gap.
// If own_values is true, frees the removed string.
void vec_str_remove(vec_str* v, size_t index);

// Returns and removes the value at the given index and closes up the
// gap. The returned str is now owned by the caller.
char* vec_str_take(vec_str* v, size_t index);

// Removes and returns the last value. Only use if v.isempty() is false.
// If own_values=true, ownership passes to the caller.
const char* vec_str_pop(vec_str* v);

// Pushes a new value onto the end of the vec, increasing the vec's size
// (and cap) if necessary.
void vec_str_push(vec_str* v, char* value);

typedef struct {
    size_t index;
    bool found;
} vec_str_find_result;

// Returns the index of value in the vec and true or 0 and false.
vec_str_find_result vec_str_find(vec_str* v, char* value);

// Returns a deep copy of the vec_str and the copy has own_values=true.
vec_str vec_str_copy(vec_str* v);

// To iterate:
//  for (size_t i = 0; i < v.size(); i++) char* value = v.get(i);
