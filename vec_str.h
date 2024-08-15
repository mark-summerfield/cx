// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vec_common.h"

// A vector of owned char* values.
// All data members are private; all accesses via functions.
typedef struct {
    size_t _size; // This is "end", i.e., one past the last value
    size_t _cap;  // The size of the allocated array
    char** _values;
} vec_str;

// Allocates a new vec_str of owned char* with default capacity of
// VEC_INITIAL_CAP.
// Set the initial capacity with cap.
vec_str vec_str_alloc_cap(size_t cap);

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
const char* vec_str_get(const vec_str* v, size_t index);

// Returns the vec_str's value at its last valid index.
// vec_str retains ownership, so do not delete the value.
const char* vec_str_get_last(const vec_str* v);

// Sets the vec_str's value at position index to the given value.
// vec_str takes ownership of the new value (e.g., if char* then use
// strdup()) and frees the old value.
void vec_str_set(vec_str* v, size_t index, char* value);

// Inserts the value at position index and moves succeeding values up
// (right), increasing the vec_str's size (and cap if necessary).
// vec_str takes ownership of the new value (e.g., if char* then use
// strdup()).
void vec_str_insert(vec_str* v, size_t index, char* value);

// Sets the vec_str's value at position index to the given value and returns
// the old value from that position.
// vec_str takes ownership of the new value (e.g., if char* then use
// strdup()). The returned value is now owned by the caller.
char* vec_str_replace(vec_str* v, size_t index, char* value);

// Removes and frees the value at the given index and closes up the gap.
void vec_str_remove(vec_str* v, size_t index);

// Returns and removes the value at the given index and closes up the
// gap.
// The returned value is now owned by the caller.
char* vec_str_take(vec_str* v, size_t index);

// Removes and returns the last value. Only use if v.isempty() is false.
// The returned value is now owned by the caller.
char* vec_str_pop(vec_str* v);

// Pushes a new value onto the end of the vec_str, increasing the vec_str's
// size (and cap if necessary). vec_str takes ownership of the value (e.g.,
// if char* then use strdup()).
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

// Returns the index of value in the vec_str and true or 0 and false.
// Uses a linear search.
vec_found_index vec_str_find(const vec_str* v, const char* value);

// Sorts the vec_str in-place.
void vec_str_sort(vec_str* v);

// Searches the vec_str using binary search: assumes that the vec_str is in
// order, e.g., vec_str_sort() has been used. For a linear search of an
// unsorted vec_str, use vec_str_find.
vec_found_index vec_str_search(const vec_str* v, const char* s);

// To iterate:
//  for (size_t i = 0; i < vec_str_size(v); ++i)
//      const MyType* value = (MyType*)vec_str_get(v, i);
