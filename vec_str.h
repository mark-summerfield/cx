// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vec_common.h"

// A vector of owns or unowned char* values.
// All data members are private; all accesses via functions.
typedef struct {
    int _size; // This is "end", i.e., one past the last value
    int _cap;  // The size of the allocated array
    char** _values;
    bool _owns;
} VecStr;

// Allocates a new VecStr of owns char* with default capacity of
// VEC_INITIAL_CAP.
#define vec_str_alloc() vec_str_alloc_custom(VEC_INITIAL_CAP, true)

// Allocates a new VecStr of char* (owns if owns, otherwise unowned)
// with the specified capacity.
VecStr vec_str_alloc_custom(int cap, bool owns);

// Allocates a new vec of owns char*'s by splitting s on sep; neither
// may be NULL.
VecStr vec_str_alloc_split(const char* s, const char* sep);

// Destroys the VecStr freeing its memory and if owns, freeing every
// value. The VecStr is not usable after this.
void vec_str_free(VecStr* vec);

// Calls free on all the VecStr's values if owns.
void vec_str_clear(VecStr* vec);

// Returns true if the VecStr owns its strings.
#define vec_str_owns(vec) ((vec)->_owns)

// Returns true if the VecStr is empty.
#define vec_str_isempty(vec) ((vec)->_size == 0)

// Returns the VecStr's size.
#define vec_str_size(vec) ((vec)->_size)

// Returns the VecStr's capacity.
#define vec_str_cap(vec) ((vec)->_cap)

// Returns the VecStr's value at position index.
// VecStr retains ownership (if owns), so do not delete the value.
char* vec_str_get(const VecStr* vec, int index);

// Returns the VecStr's value at its last valid index.
// VecStr retains ownership (if owns), so do not delete the value.
char* vec_str_get_last(const VecStr* vec);

// Sets the VecStr's value at position index to the given value.
// If owns, VecStr takes ownership of the new value (e.g., if char*
// then use strdup()) and frees the old value.
void vec_str_set(VecStr* vec, int index, char* value);

// Inserts the value at position index and moves succeeding values up
// (right), increasing the VecStr's size (and cap if necessary): O(n).
// If owns, VecStr takes ownership of the new value (e.g., use strdup()).
void vec_str_insert(VecStr* vec, int index, char* value);

// Adds the value in order (in a sorted vec) and moves succeeding values up
// (right), increasing the vec's size (and cap if necessary): O(n).
// If owns, vec takes ownership of the new value (e.g., use strdup()).
void vec_str_add(VecStr* vec, char* value);

// Sets the VecStr's value at position index to the given value and returns
// the old value from that position.
// If owns, VecStr takes ownership of the new value (e.g., if char*
// then use strdup()). The returned value is now owns by the caller (if
// it was owns by the VecStr).
char* vec_str_replace(VecStr* vec, int index, char* value);

// Removes (and frees if owns) the value at the given index and closes
// up the gap: O(n).
void vec_str_remove(VecStr* vec, int index);

// Returns and removes the value at the given index and closes up the
// gap: O(n).
// The returned value is now owns by the caller if it was owns by VecStr.
char* vec_str_take(VecStr* vec, int index);

// Removes and returns the last value. Only use if vec.isempty() is false.
// The returned value is now owns by the caller if it was owns by
// VecStr: O(1).
char* vec_str_pop(VecStr* vec);

// Pushes a new value onto the end of the VecStr, increasing the VecStr's
// size (and cap if necessary): O(1). If owns, VecStr takes ownership
// of the value (e.g., if char* then use strdup()).
void vec_str_push(VecStr* vec, char* value);

// Returns a copy of the VecStr (a deep copy if owns).
VecStr vec_str_copy(const VecStr* vec);

// Moves all vec2's values to the end of vec1's values, after which vec2 is
// freed and must not be used again.
// Use case: an array of VecStr's each one of which is populated in its own
// thread and at the end we want to merge all the VecStr's into one.
void vec_str_merge(VecStr* vec1, VecStr* vec2);

// Returns true if the two VecStr's have the same values.
bool vec_str_equal(const VecStr* vec1, const VecStr* vec2);

// Returns a char* of all the vec's strings joined together with sep, or
// NULL if vec is empty. If sep is NULL they are joined with no separator.
// The returned char* value is now owns by the caller.
char* vec_str_join(const VecStr* vec, const char* sep);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_str_find(const VecStr* vec, const char* value);

// Returns the last index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_str_find_last(const VecStr* vec, const char* value);

// Sorts the VecStr in-place.
void vec_str_sort(VecStr* vec);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a binary search that assumes vec_str_sort() has
// been used.
int vec_str_search(const VecStr* vec, const char* s);

// To iterate:
//      for (int i = 0; i < vec_str_size(vec); ++i)
//          const char* value = vec_str_get(vec, i);
