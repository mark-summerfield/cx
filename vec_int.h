#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx.h"
#include "vec_common.h"
#include <stdio.h>

// Change to long or any other integer type if needed.
typedef int VEC_INT_VALUE_T;

// A vector of VEC_INT_VALUE_T values.
// All data members are private; all accesses via functions.
typedef struct vec_int {
    size_t _size; // This is "end", i.e., one past the last value
    size_t _cap;  // The size of the allocated array
    VEC_INT_VALUE_T* _values;
} vec_int;

// Allocates a new empty vec_int with the given capacity.
vec_int vec_int_alloc_cap(size_t cap);

// Allocates a new empty vec_int with a default capacity of
// VEC_INITIAL_CAP.
#define vec_int_alloc() vec_int_alloc_cap(VEC_INITIAL_CAP)

// Destroys the vec_int freeing its memory. The vec_int is not usable
// after this.
void vec_int_free(vec_int* v);

// Clears the vec_int freeing its memory. The vec_int is usable after
// this (e.g., push() etc).
void vec_int_clear(vec_int* v);

// Returns true if the vec_int is empty.
#define vec_int_isempty(v) ((v)->_size > 0)

// Returns the vec_int's size.
#define vec_int_size(v) ((v)->_size)

// Returns the vec_int's capacity.
#define vec_int_cap(v) ((v)->_cap)

// Returns the vec_int's VEC_INT_VALUE_T value at position index.
VEC_INT_VALUE_T vec_int_get(const vec_int* v, size_t index);

// Returns the vec_int's VEC_INT_VALUE_T value at its last valid index.
VEC_INT_VALUE_T vec_int_get_last(const vec_int* v);

// Sets the vec_int's value at position index to the given VEC_INT_VALUE_T.
void vec_int_set(vec_int* v, size_t index, VEC_INT_VALUE_T value);

// Inserts the VEC_INT_VALUE_T at position index and moves succeeding values
// up (right), increasing the vec_int's size (and cap if necessary).
void vec_int_insert(vec_int* v, size_t index, VEC_INT_VALUE_T value);

// Sets the vec_int's value at position index to the given VEC_INT_VALUE_T
// and returns the old VEC_INT_VALUE_T value from that position.
VEC_INT_VALUE_T vec_int_replace(vec_int* v, size_t index,
                                VEC_INT_VALUE_T value);

// Removes the value at the given index and closes up the gap.
void vec_int_remove(vec_int* v, size_t index);

// Returns and removes the VEC_INT_VALUE_T value at the given index and
// closes up the gap.
VEC_INT_VALUE_T vec_int_take(vec_int* v, size_t index);

// Removes and returns the last VEC_INT_VALUE_T value. Only use if
// v.isempty() is false.
VEC_INT_VALUE_T vec_int_pop(vec_int* v);

// Pushes a new VEC_INT_VALUE_T value onto the end of the vec_int,
// increasing the vec_int's size (and cap) if necessary.
void vec_int_push(vec_int* v, VEC_INT_VALUE_T value);

// Returns the index of the VEC_INT_VALUE_T value in the vec_int and true or
// 0 and false.
vec_found_index vec_int_find(const vec_int* v, VEC_INT_VALUE_T value);

// Returns a deep copy of the vec_int.
vec_int vec_int_copy(const vec_int* v);

// Returns true if the two vec_int's have the same VEC_INT_VALUE_T values.
bool vec_int_equal(const vec_int* v1, const vec_int* v2);

// Sorts the vec_int in-place in ascending order.
void vec_int_sort(vec_int* v);

// Searches the vec_int using binary search: assumes that the vec_int is
// in order, e.g., vec_int_sort() has been used. For a linear search of
// an unsorted vec_int, use vec_int_find.
vec_found_index vec_int_search(const vec_int* v, int i);

// Returns a string of space-separated VEC_INT_VALUE_T values.
// The returned char* value is now owned by the caller.
char* vec_int_tostring(const vec_int* v);

// To iterate:
//  for (size_t i = 0; i < vec_int_size(v); ++i)
//      VEC_INT_VALUE_T value = vec_int_get(v, i);
