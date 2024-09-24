// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vecs.h"
#include <stdbool.h>
#include <stdio.h>

// A vector of int values.
// All accesses via functions, but _reading_ `_values` is okay.
typedef struct VecInt {
    int _size; // This is "end", i.e., one past the last value
    int _cap;  // The size of the allocated array
    int* _values;
} VecInt;

// Allocates a new empty VecInt with the given capacity.
VecInt vec_int_alloc_cap(int cap);

// Allocates a new empty VecInt with a default capacity of 0.
#define vec_int_alloc() vec_int_alloc_cap(0)

// Destroys the VecInt freeing its memory. The VecInt is not usable
// after this.
void vec_int_free(VecInt* vec);

// Clears all the VecInt's values.
void vec_int_clear(VecInt* vec);

// Returns true if the VecInt is empty.
#define vec_int_isempty(vec) ((vec)->_size == 0)

// Returns the VecInt's size.
#define vec_int_size(vec) ((vec)->_size)

// Returns the VecInt's capacity.
#define vec_int_cap(vec) ((vec)->_cap)

// Returns the VecInt's int value at position index.
// The VEC_GET() macro is faster but unchecked.
int vec_int_get(const VecInt* vec, int index);

// Returns the VecInt's int value at its first valid index.
// The VEC_GET_FIRST() macro is faster but unchecked.
int vec_int_get_first(const VecInt* vec);

// Returns the VecInt's int value at its last valid index.
// The VEC_GET_LAST() macro is faster but unchecked.
int vec_int_get_last(const VecInt* vec);

// Sets the VecInt's value at position index to the given int.
void vec_int_set(VecInt* vec, int index, int value);

// Inserts the int at position index and moves succeeding values
// up (right), increasing the VecInt's size (and cap if necessary): O(n).
void vec_int_insert(VecInt* vec, int index, int value);

// Adds the value in order (in a sorted vec) and moves succeeding values up
// (right), increasing the vec's size (and cap if necessary): O(n).
void vec_int_add(VecInt* vec, int value);

// Sets the VecInt's value at position index to the given int
// and returns the old int value from that position.
int vec_int_replace(VecInt* vec, int index, int value);

// Removes the value at the given index and closes up the gap: O(n).
void vec_int_remove(VecInt* vec, int index);

// Returns and removes the value at the given index and closes up the
// gap: O(n).
int vec_int_take(VecInt* vec, int index);

// Removes and returns the last int value. Only use if
// vec.isempty() is false: O(1).
int vec_int_pop(VecInt* vec);

// Pushes a new int value onto the end of the VecInt,
// increasing the VecInt's size (and cap) if necessary: O(1).
void vec_int_push(VecInt* vec, int value);

// Returns a deep copy of the VecInt.
VecInt vec_int_copy(const VecInt* vec);

// Moves all vec2's values to the end of vec1's values, after which vec2 is
// freed and must not be used again.
// The two VecInt's must have the same cmp, cpy, and destroy methods.
// Use case: an array of VecInt's each one of which is populated in its own
// thread and at the end we want to merge all the VecInt's into one.
void vec_int_merge(VecInt* vec1, VecInt* vec2);

// Returns true if the two VecInt's have the same int values.
bool vec_int_equal(const VecInt* vec1, const VecInt* vec2);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_int_find(const VecInt* vec, int value);

// Returns the last index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_int_find_last(const VecInt* vec, int value);

// Sorts the VecInt in-place in ascending order.
void vec_int_sort(VecInt* vec);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a binary search that assumes vec_int_sort() has
// been used.
int vec_int_search(const VecInt* vec, int i);

// Returns a string of space-separated int values.
// The returned char* value is now owns by the caller.
char* vec_int_to_str(const VecInt* vec);

// For debugging.
void vec_int_dump(const VecInt* vec);

// To iterate:
//      for (int i = 0; i < vec_int_size(vec); ++i)
//          int value = VEC_GET(vec, i);
