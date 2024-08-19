// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vec_common.h"

// A vector of owned char* values.
// All data members are private; all accesses via functions.
typedef struct {
    SSIZE_T _size; // This is "end", i.e., one past the last value
    SSIZE_T _cap;  // The size of the allocated array
    char** _values;
} vec_str;

// Allocates a new vec_str of owned char* with default capacity of
// VEC_INITIAL_CAP.
// Set the initial capacity with cap.
vec_str vec_str_alloc_cap(SSIZE_T cap);

#define vec_str_alloc() vec_str_alloc_cap(VEC_INITIAL_CAP)

// Allocates a new vec of owned char*'s by splitting s on sep; neither
// may be NULL.
vec_str vec_str_alloc_split(const char* s, const char* sep);

// Destroys the vec_str freeing its memory and also freeing every value. The
// vec_str is not usable after this.
void vec_str_free(vec_str* v);

// Calls free on all the vec_str's values.
void vec_str_clear(vec_str* v);

// Returns true if the vec_str is empty.
#define vec_str_isempty(v) ((v)->_size == 0)

// Returns the vec_str's size.
#define vec_str_size(v) ((v)->_size)

// Returns the vec_str's capacity.
#define vec_str_cap(v) ((v)->_cap)

// Returns the vec_str's value at position index.
// vec_str retains ownership, so do not delete the value.
const char* vec_str_get(const vec_str* v, SSIZE_T index);

// Returns the vec_str's value at its last valid index.
// vec_str retains ownership, so do not delete the value.
const char* vec_str_get_last(const vec_str* v);

// Sets the vec_str's value at position index to the given value.
// vec_str takes ownership of the new value (e.g., if char* then use
// strdup()) and frees the old value.
void vec_str_set(vec_str* v, SSIZE_T index, char* value);

// Inserts the value at position index and moves succeeding values up
// (right), increasing the vec_str's size (and cap if necessary): O(n).
// vec_str takes ownership of the new value (e.g., use strdup()).
void vec_str_insert(vec_str* v, SSIZE_T index, char* value);

// Adds the value in order (in a sorted vec) and moves succeeding values up
// (right), increasing the vec's size (and cap if necessary): O(n).
// vec takes ownership of the new value (e.g., use strdup()).
void vec_str_add(vec_str* v, char* value);

// Sets the vec_str's value at position index to the given value and returns
// the old value from that position.
// vec_str takes ownership of the new value (e.g., if char* then use
// strdup()). The returned value is now owned by the caller.
char* vec_str_replace(vec_str* v, SSIZE_T index, char* value);

// Removes and frees the value at the given index and closes up the gap:
// O(n).
void vec_str_remove(vec_str* v, SSIZE_T index);

// Returns and removes the value at the given index and closes up the
// gap: O(n).
// The returned value is now owned by the caller.
char* vec_str_take(vec_str* v, SSIZE_T index);

// Removes and returns the last value. Only use if v.isempty() is false.
// The returned value is now owned by the caller: O(1).
char* vec_str_pop(vec_str* v);

// Pushes a new value onto the end of the vec_str, increasing the vec_str's
// size (and cap if necessary): O(1). vec_str takes ownership of the value
// (e.g., if char* then use strdup()).
void vec_str_push(vec_str* v, char* value);

// Returns a deep copy of the vec_str.
vec_str vec_str_copy(const vec_str* v);

// Moves all v2's values to the end of v1's values, after which v2 is
// freed and must not be used again.
// Use case: an array of vec_str's each one of which is populated in its own
// thread and at the end we want to merge all the vec_str's into one.
void vec_str_merge(vec_str* v1, vec_str* v2);

// Returns true if the two vec_str's have the same values.
bool vec_str_equal(const vec_str* v1, const vec_str* v2);

// Returns a char* of all the vec's strings joined together with sep. If
// sep is NULL they are joined with no separator.
// The returned char* value is now owned by the caller.
char* vec_str_join(const vec_str* v, const char* sep);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
SSIZE_T vec_str_find(const vec_str* v, const char* value);

// Sorts the vec_str in-place.
void vec_str_sort(vec_str* v);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a binary search that assumes vec_str_sort() has
// been used.
SSIZE_T vec_str_search(const vec_str* v, const char* s);

// To iterate:
//  for (SSIZE_T i = 0; i < vec_str_size(v); ++i)
//      const char* value = vec_str_get(v, i);
