// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vec_common.h"
#include <stdio.h>

// Change to long or any other integer type if needed.
typedef int VEC_INT_VALUE_T;

// A vector of VEC_INT_VALUE_T values.
// All data members are private; all accesses via functions.
typedef struct {
    cx_size _size; // This is "end", i.e., one past the last value
    cx_size _cap;  // The size of the allocated array
    VEC_INT_VALUE_T* _values;
} vec_int;

// Allocates a new empty vec_int with the given capacity.
vec_int vec_int_alloc_cap(cx_size cap);

// Allocates a new empty vec_int with a default capacity of
// VEC_INITIAL_CAP.
#define vec_int_alloc() vec_int_alloc_cap(VEC_INITIAL_CAP)

// Destroys the vec_int freeing its memory. The vec_int is not usable
// after this.
void vec_int_free(vec_int* v);

// Clears all the vec_int's values.
void vec_int_clear(vec_int* v);

// Returns true if the vec_int is empty.
#define vec_int_isempty(v) ((v)->_size == 0)

// Returns the vec_int's size.
#define vec_int_size(v) ((v)->_size)

// Returns the vec_int's capacity.
#define vec_int_cap(v) ((v)->_cap)

// Returns the vec_int's VEC_INT_VALUE_T value at position index.
VEC_INT_VALUE_T vec_int_get(const vec_int* v, cx_size index);

// Returns the vec_int's VEC_INT_VALUE_T value at its last valid index.
VEC_INT_VALUE_T vec_int_get_last(const vec_int* v);

// Sets the vec_int's value at position index to the given VEC_INT_VALUE_T.
void vec_int_set(vec_int* v, cx_size index, VEC_INT_VALUE_T value);

// Inserts the VEC_INT_VALUE_T at position index and moves succeeding values
// up (right), increasing the vec_int's size (and cap if necessary): O(n).
void vec_int_insert(vec_int* v, cx_size index, VEC_INT_VALUE_T value);

// Adds the value in order (in a sorted vec) and moves succeeding values up
// (right), increasing the vec's size (and cap if necessary): O(n).
void vec_int_add(vec_int* v, VEC_INT_VALUE_T value);

// Sets the vec_int's value at position index to the given VEC_INT_VALUE_T
// and returns the old VEC_INT_VALUE_T value from that position.
VEC_INT_VALUE_T vec_int_replace(vec_int* v, cx_size index,
                                VEC_INT_VALUE_T value);

// Removes the value at the given index and closes up the gap: O(n).
void vec_int_remove(vec_int* v, cx_size index);

// Returns and removes the VEC_INT_VALUE_T value at the given index and
// closes up the gap: O(n).
VEC_INT_VALUE_T vec_int_take(vec_int* v, cx_size index);

// Removes and returns the last VEC_INT_VALUE_T value. Only use if
// v.isempty() is false: O(1).
VEC_INT_VALUE_T vec_int_pop(vec_int* v);

// Pushes a new VEC_INT_VALUE_T value onto the end of the vec_int,
// increasing the vec_int's size (and cap) if necessary: O(1).
void vec_int_push(vec_int* v, VEC_INT_VALUE_T value);

// Returns a deep copy of the vec_int.
vec_int vec_int_copy(const vec_int* v);

// Moves all v2's values to the end of v1's values, after which v2 is
// freed and must not be used again.
// The two vec_int's must have the same cmp, cpy, and destroy methods.
// Use case: an array of vec_int's each one of which is populated in its own
// thread and at the end we want to merge all the vec_int's into one.
void vec_int_merge(vec_int* v1, vec_int* v2);

// Returns true if the two vec_int's have the same VEC_INT_VALUE_T values.
bool vec_int_equal(const vec_int* v1, const vec_int* v2);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
cx_size vec_int_find(const vec_int* v, VEC_INT_VALUE_T value);

// Returns the last index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
cx_size vec_int_find_last(const vec_int* v, VEC_INT_VALUE_T value);

VEC_INT_VALUE_T intcmp(const void* a, const void* b);

// Sorts the vec_int in-place in ascending order.
void vec_int_sort(vec_int* v);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a binary search that assumes vec_int_sort() has
// been used.
cx_size vec_int_search(const vec_int* v, int i);

// Returns a string of space-separated VEC_INT_VALUE_T values.
// The returned char* value is now owned by the caller.
char* vec_int_tostring(const vec_int* v);

// To iterate:
//      for (cx_size i = 0; i < vec_int_size(v); ++i)
//          int value = vec_int_get(v, i);
