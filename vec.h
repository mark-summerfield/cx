// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vec_common.h"

// A vector of owned void* values.
// All data members are private; all accesses via functions.
//
// To see an example of how to use store an arbitrary struct type in a
// vec see tag_test.h for the Tag struct and vec_test.[hc] for usage
// examples.
typedef struct {
    int _size; // This is "end", i.e., one past the last value
    int _cap;  // The size of the allocated array
    void** _values;
    int (*_cmp)(const void*, const void*);
    void* (*_cpy)(const void*);
    void (*_destroy)(void* value);
} vec;

typedef struct {
    int cap;
    int (*cmp)(const void*, const void*);
    void* (*cpy)(const void*);
    void (*destroy)(void* values);
} vec_alloc_args;

// Allocates a new vec of owned void* with default capacity of
// VEC_INITIAL_CAP.
// Set the initial capacity with .cap.
// Caller must supply functions: cmp to compare values (for find, sort,
// and search), cpy to copy a value, and destroy to free a value.
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
const void* vec_get(const vec* v, int index);

// Returns the vec's value at its last valid index.
// vec retains ownership, so do not delete the value.
const void* vec_get_last(const vec* v);

// Sets the vec's value at position index to the given value.
// vec takes ownership of the new value (e.g., if char* then use strdup())
// and frees the old value.
void vec_set(vec* v, int index, void* value);

// Inserts the value at position index and moves succeeding values up
// (right), increasing the vec's size (and cap if necessary): O(n).
// Use add to insert into a sorted vec.
// vec takes ownership of the new value (e.g., if char* then use strdup()).
void vec_insert(vec* v, int index, void* value);

// Adds the value in order (in a sorted vec) and moves succeeding values up
// (right), increasing the vec's size (and cap if necessary): O(n).
// vec takes ownership of the new value (e.g., if char* then use strdup()).
void vec_add(vec* v, void* value);

// Sets the vec's value at position index to the given value and returns
// the old value from that position.
// vec takes ownership of the new value (e.g., if char* then use strdup()).
// The returned value is now owned by the caller.
void* vec_replace(vec* v, int index, void* value);

// Removes and frees the value at the given index and closes up the gap:
// O(n).
void vec_remove(vec* v, int index);

// Returns and removes the value at the given index and closes up the
// gap.
// The returned value is now owned by the caller: O(n).
void* vec_take(vec* v, int index);

// Removes and returns the last value. Only use if v.isempty() is false.
// The returned value is now owned by the caller: O(1).
void* vec_pop(vec* v);

// Pushes a new value onto the end of the vec, increasing the vec's size
// (and cap if necessary): O(1).
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

// Returns true if the two vec's have the same values and the same cmp,
// cpy, and destroy.
bool vec_equal(const vec* v1, const vec* v2);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_find(const vec* v, const void* value);

// Returns the last index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_find_last(const vec* v, const void* value);

// Sorts the vec in-place using the cmp function.
// See tag_test.h's tag_cmp and sx.c's sx_strcmp functions for examples
// of how to create a cmp function.
void vec_sort(vec* v);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a binary search that assumes vec_sort() has
// been used.
int vec_search(const vec* v, const void* value);

// To iterate:
//      for (int i = 0; i < vec_size(v); ++i)
//          const MyType* value = vec_get(v, i);
