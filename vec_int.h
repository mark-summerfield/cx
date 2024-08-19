// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vec_common.h"
#include <stdio.h>

// A vector of int values.
// All data members are private; all accesses via functions.
typedef struct {
    int _size; // This is "end", i.e., one past the last value
    int _cap;  // The size of the allocated array
    int* _values;
} vec_int;

// Allocates a new empty vec_int with the given capacity.
vec_int vec_int_alloc_cap(int cap);

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

// Returns the vec_int's int value at position index.
int vec_int_get(const vec_int* v, int index);

// Returns the vec_int's int value at its last valid index.
int vec_int_get_last(const vec_int* v);

// Sets the vec_int's value at position index to the given int.
void vec_int_set(vec_int* v, int index, int value);

// Inserts the int at position index and moves succeeding values
// up (right), increasing the vec_int's size (and cap if necessary): O(n).
void vec_int_insert(vec_int* v, int index, int value);

// Adds the value in order (in a sorted vec) and moves succeeding values up
// (right), increasing the vec's size (and cap if necessary): O(n).
void vec_int_add(vec_int* v, int value);

// Sets the vec_int's value at position index to the given int
// and returns the old int value from that position.
int vec_int_replace(vec_int* v, int index, int value);

// Removes the value at the given index and closes up the gap: O(n).
void vec_int_remove(vec_int* v, int index);

// Returns and removes the int value at the given index and
// closes up the gap: O(n).
int vec_int_take(vec_int* v, int index);

// Removes and returns the last int value. Only use if
// v.isempty() is false: O(1).
int vec_int_pop(vec_int* v);

// Pushes a new int value onto the end of the vec_int,
// increasing the vec_int's size (and cap) if necessary: O(1).
void vec_int_push(vec_int* v, int value);

// Returns a deep copy of the vec_int.
vec_int vec_int_copy(const vec_int* v);

// Moves all v2's values to the end of v1's values, after which v2 is
// freed and must not be used again.
// The two vec_int's must have the same cmp, cpy, and destroy methods.
// Use case: an array of vec_int's each one of which is populated in its own
// thread and at the end we want to merge all the vec_int's into one.
void vec_int_merge(vec_int* v1, vec_int* v2);

// Returns true if the two vec_int's have the same int values.
bool vec_int_equal(const vec_int* v1, const vec_int* v2);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_int_find(const vec_int* v, int value);

// Returns the last index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_int_find_last(const vec_int* v, int value);

int intcmp(const void* a, const void* b);

// Sorts the vec_int in-place in ascending order.
void vec_int_sort(vec_int* v);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a binary search that assumes vec_int_sort() has
// been used.
int vec_int_search(const vec_int* v, int i);

// Returns a string of space-separated int values.
// The returned char* value is now owned by the caller.
char* vec_int_tostring(const vec_int* v);

// To iterate:
//      for (int i = 0; i < vec_int_size(v); ++i)
//          int value = vec_int_get(v, i);
