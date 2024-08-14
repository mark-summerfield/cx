#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx.h"
#include "vec_common.h"

// A vector of owned void* values.
// All data members are private; all accesses via functions.
typedef struct {
    size_t _size; // This is "end", i.e., one past the last value
    size_t _cap;  // The size of the allocated array
    void** _values;
    int (*_cmp)(const void* a, const void* b);
    void* (*_cpy)(const void*);
    void (*_destroy)(void* value);
} vec;

typedef struct {
    size_t cap;
    int (*cmp)(const void* a, const void* b);
    void* (*cpy)(const void*);
    void (*destroy)(void* values);
} vec_alloc_args;

// Allocates a new vec of owned void* with default capacity of
// VEC_INITIAL_CAP.
// Set the initial capacity with .cap.
// Caller must supply functions: cmp to compare two values, cpy to copy a
// value, and destroy to free a value.
#define vec_alloc(...) \
    vec_alloc_((vec_alloc_args){.cap = VEC_INITIAL_CAP, __VA_ARGS__})
vec vec_alloc_(vec_alloc_args args);

// Destroys the vec freeing its memory and also freeing every value. The vec
// is not usable after this.
void vec_free(vec* v);

// Calls destroy on all the vec's values.
void vec_clear(vec* v);

// Returns true if the vec is empty.
#define vec_isempty(v) ((v)->_size == 0)

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

// Returns a deep copy of the vec including cmp, cpy, and destroy.
vec vec_copy(const vec* v);

// Moves all v2's values to the end of v1's values, after which v2 is
// freed and must not be used again.
// The two vec's must have the same cmp, cpy, and destroy methods.
// Use case: an array of vec's each one of which is populated in its own
// thread and at the end we want to merge all the vec's into one.
void vec_merge(vec* v1, vec* v2);

// Returns true if the two vec's have the same values and cmp, cpy, and
// destroy functions.
bool vec_equal(const vec* v1, const vec* v2);

// Returns the index of value in the vec and true or 0 and false.
// Uses a linear search.
vec_found_index vec_find(const vec* v, const void* value);

// Sorts the vec in-place using the cmp function.
void vec_sort(vec* v);

// Searches the vec using binary search: assumes that the vec is in
// order, e.g., vec_sort() has been used with the cmp function. For a
// linear search of an unsorted vec, use vec_str_find.
vec_found_index vec_str_search(const vec* v, const void* s);

// To iterate:
//  for (size_t i = 0; i < vec_size(v); ++i)
//      const MyType* value = (MyType*)vec_get(v, i);
