// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vec_common.h"

// A vector of owned char* values.
// All data members are private; all accesses via functions.
typedef struct {
    int _size; // This is "end", i.e., one past the last value
    int _cap;  // The size of the allocated array
    char** _values;
} vec_str;

// Allocates a new vec_str of owned char* with default capacity of
// VEC_INITIAL_CAP.
// Set the initial capacity with cap.
vec_str vec_str_alloc_cap(int cap);

#define vec_str_alloc() vec_str_alloc_cap(VEC_INITIAL_CAP)

// Allocates a new vec of owned char*'s by splitting s on sep; neither
// may be NULL.
vec_str vec_str_alloc_split(const char* s, const char* sep);

// Destroys the vec_str freeing its memory and also freeing every value. The
// vec_str is not usable after this.
void vec_str_free(vec_str* me);

// Calls free on all the vec_str's values.
void vec_str_clear(vec_str* me);

// Returns true if the vec_str is empty.
#define vec_str_isempty(me) ((me)->_size == 0)

// Returns the vec_str's size.
#define vec_str_size(me) ((me)->_size)

// Returns the vec_str's capacity.
#define vec_str_cap(me) ((me)->_cap)

// Returns the vec_str's value at position index.
// vec_str retains ownership, so do not delete the value.
const char* vec_str_get(const vec_str* me, int index);

// Returns the vec_str's value at its last valid index.
// vec_str retains ownership, so do not delete the value.
const char* vec_str_get_last(const vec_str* me);

// Sets the vec_str's value at position index to the given value.
// vec_str takes ownership of the new value (e.g., if char* then use
// strdup()) and frees the old value.
void vec_str_set(vec_str* me, int index, char* value);

// Inserts the value at position index and moves succeeding values up
// (right), increasing the vec_str's size (and cap if necessary): O(n).
// vec_str takes ownership of the new value (e.g., use strdup()).
void vec_str_insert(vec_str* me, int index, char* value);

// Adds the value in order (in a sorted vec) and moves succeeding values up
// (right), increasing the vec's size (and cap if necessary): O(n).
// vec takes ownership of the new value (e.g., use strdup()).
void vec_str_add(vec_str* me, char* value);

// Sets the vec_str's value at position index to the given value and returns
// the old value from that position.
// vec_str takes ownership of the new value (e.g., if char* then use
// strdup()). The returned value is now owned by the caller.
char* vec_str_replace(vec_str* me, int index, char* value);

// Removes and frees the value at the given index and closes up the gap:
// O(n).
void vec_str_remove(vec_str* me, int index);

// Returns and removes the value at the given index and closes up the
// gap: O(n).
// The returned value is now owned by the caller.
char* vec_str_take(vec_str* me, int index);

// Removes and returns the last value. Only use if me.isempty() is false.
// The returned value is now owned by the caller: O(1).
char* vec_str_pop(vec_str* me);

// Pushes a new value onto the end of the vec_str, increasing the vec_str's
// size (and cap if necessary): O(1). vec_str takes ownership of the value
// (e.g., if char* then use strdup()).
void vec_str_push(vec_str* me, char* value);

// Returns a deep copy of the vec_str.
vec_str vec_str_copy(const vec_str* me);

// Moves all them's values to the end of me's values, after which them is
// freed and must not be used again.
// Use case: an array of vec_str's each one of which is populated in its own
// thread and at the end we want to merge all the vec_str's into one.
void vec_str_merge(vec_str* me, vec_str* them);

// Returns true if the two vec_str's have the same values.
bool vec_str_equal(const vec_str* me, const vec_str* them);

// Returns a char* of all the vec's strings joined together with sep. If
// sep is NULL they are joined with no separator.
// The returned char* value is now owned by the caller.
char* vec_str_join(const vec_str* me, const char* sep);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_str_find(const vec_str* me, const char* value);

// Returns the last index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_str_find_last(const vec_str* me, const char* value);

// Sorts the vec_str in-place.
void vec_str_sort(vec_str* me);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a binary search that assumes vec_str_sort() has
// been used.
int vec_str_search(const vec_str* me, const char* s);

// To iterate:
//      for (int i = 0; i < vec_str_size(me); ++i)
//          const char* value = vec_str_get(me, i);
