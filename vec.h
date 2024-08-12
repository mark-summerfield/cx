#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx.h"
#include "vec_common.h"

// A vector of owned void* values.
// All data members are private; all accesses via functions.
typedef struct vec {
    size_t _size; // This is "end", i.e., one past the last value
    size_t _cap;  // The size of the allocated array
    void** _values;
    bool (*_eq)(void* a, void* b);
    void* (*_cp)(void*);
    void (*_destroy)(void* value);
} vec;

typedef struct {
    size_t cap;
    bool (*eq)(void* a, void* b);
    void* (*cp)(void*);
    void (*destroy)(void* values);
} vec_alloc_args;

// Allocates a new vec of owned void* with default capacity of
// VEC_INITIAL_SIZE.
// Set the initial capacity with .cap.
// Caller must supply functions: eq to compare two values, cp to deep
// copy a value, and a destroy function to free a value.
#define vec_alloc(...) \
    vec_alloc_((vec_alloc_args){.cap = VEC_INITIAL_SIZE, __VA_ARGS__})
vec vec_alloc_(vec_alloc_args args);

// Destroys the vec freeing its memory and also freeing every value. The vec
// is not usable after this.
void vec_free(vec* v);

// Clears the vec freeing its memory and also freeing every value. The vec
// is usable after this (e.g., push() etc).
void vec_clear(vec* v);

// Returns true if the vec is empty.
#define vec_isempty(v) ((v)->_size > 0)

// Returns the vec's size.
#define vec_size(v) ((v)->_size)

// Returns the vec's capacity.
#define vec_cap(v) ((v)->_cap)

// Returns the vec's value at position index.
// vec retains ownership, so do not delete the value.
const void* vec_get(const vec* v, size_t index);

// Returns the vec's value at its last valid index.
// vec retains ownership, so do not delete the value.
const void* vec_get_last(const vec* v);

// Sets the vec's value at position index to the given value.
// vec takes ownership of the new value (e.g., if char* then use strdup())
// and frees the old value.
void vec_set(vec* v, size_t index, void* value);

// Inserts the value at position index and moves succeeding values up
// (right), increasing the vec's size (and cap if necessary).
// vec takes ownership of the new value (e.g., if char* then use strdup()).
void vec_insert(vec* v, size_t index, void* value);

// Sets the vec's value at position index to the given value and returns
// the old value from that position.
// vec takes ownership of the new value (e.g., if char* then use strdup()).
// The returned value is now owned by the caller.
void* vec_replace(vec* v, size_t index, void* value);

// Removes and frees the value at the given index and closes up the gap.
void vec_remove(vec* v, size_t index);

// Returns and removes the value at the given index and closes up the
// gap.
// The returned value is now owned by the caller.
void* vec_take(vec* v, size_t index);

// Removes and returns the last value. Only use if v.isempty() is false.
// The returned value is now owned by the caller.
void* vec_pop(vec* v);

// Pushes a new value onto the end of the vec, increasing the vec's size
// (and cap if necessary).
// vec takes ownership of the value (e.g., if char* then use strdup()).
void vec_push(vec* v, void* value);

// Returns the index of value in the vec and true or 0 and false.
// Uses a linear search.
vec_found_index vec_find(const vec* v, void* value);

// Returns a deep copy of the vec including eq, cp, and destroy.
vec vec_copy(const vec* v);

// Returns true if the two vec's have the same values and eq, cp, and
// destroy functions.
bool vec_equal(const vec* v1, const vec* v2);

// To iterate:
//  for (size_t i = 0; i < vec_size(v); ++i)
//      const MyType* value = (MyType*)vec_get(v, i);
