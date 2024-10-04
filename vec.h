// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vecs.h"
#include <stdbool.h>

// A vector of owned or borrowed void* values.
// All accesses via functions, but _reading_ `_values` is okay.
// See vecs.h for size and capacity macros.
//
// To see an example of how to use store an arbitrary struct type in a
// Vec see tag_test.h for the Tag struct and vec_test.[hc] for usage
// examples.
//
// To iterate:
// ```
//  for (int i = 0; i < VEC_SIZE(vec); ++i)
//      const MyType* value = VEC_GET(vec, i);
// ```
typedef struct Vec {
    int _size; // This is "end", i.e., one past the last value
    int _cap;  // The size of the allocated array
    void** _values;
    int (*_cmp)(const void*, const void*);
    void (*_destroy)(void* value);
} Vec;

// Allocates a new Vec of owned or borrowed void* with the given
// capacity which may be 0.
// Caller must supply cmp to compare values (for find, sort, and search),
// and if owning, destroy to free a value.
Vec vec_alloc(int cap, int (*cmp)(const void*, const void*),
              void (*destroy)(void* value));

// Destroys the Vec freeing its memory and if owning, also freeing every
// value. The Vec is not usable after this.
void vec_free(Vec* vec);

// Calls destroy on all the Vec's values if owning.
void vec_clear(Vec* vec);

// Returns true if the Vec is owning.
#define vec_owns(vec) ((vec)->_destroy != NULL)

// Returns the Vec's value at position index.
// If owning, Vec retains ownership, so do not delete the value.
// The VEC_GET() macro is faster but unchecked.
void* vec_get(const Vec* vec, int index);

// Returns the Vec's value at its first valid index.
// If owning, Vec retains ownership, so do not delete the value.
// The VEC_GET_FIRST() macro is faster but unchecked.
void* vec_get_first(const Vec* vec);

// Returns the Vec's value at its last valid index.
// If owning, Vec retains ownership, so do not delete the value.
// The VEC_GET_LAST() macro is faster but unchecked.
void* vec_get_last(const Vec* vec);

// Sets the Vec's value at position index to the given value.
// If owning, Vec takes ownership of the new value (e.g., if char* then use
// strdup()) and frees the old value.
void vec_set(Vec* vec, int index, void* value);

// Inserts the value at position index and moves succeeding values up
// (right), increasing the Vec's size (and cap if necessary): O(n).
// Use add to insert into a sorted Vec, or push to insert at the end of
// an unsorted Vec.
// If owning, Vec takes ownership of the new value (e.g., if char* then use
// strdup()).
void vec_insert(Vec* vec, int index, void* value);

// Adds the value in order (in a sorted Vec) and moves succeeding values up
// (right), increasing the Vec's size (and cap if necessary): O(n).
// If owning, Vec takes ownership of the new value (e.g., if char* then use
// strdup()).
void vec_add(Vec* vec, void* value);

// Sets the Vec's value at position index to the given value and returns
// the old value from that position.
// If owning, Vec takes ownership of the new value (e.g., if char* then use
// strdup()). The returned value is now owned by the caller if it was owned
// by Vec.
void* vec_replace(Vec* vec, int index, void* value);

// Removes and, if owning, frees the value at the given index and closes up
// the gap: O(n).
void vec_remove(Vec* vec, int index);

// Returns and removes the value at the given index and closes up the
// gap.
// If Vec is owning, the returned value is now owned by the caller: O(n).
void* vec_take(Vec* vec, int index);

// Removes and returns the last value. Only use if vec.isempty() is false.
// The returned value is now owned by the caller, if Vec is owning: O(1).
void* vec_pop(Vec* vec);

// Pushes a new value onto the end of the Vec, increasing the Vec's size
// (and cap if necessary): O(1). Use vec_add() to insert into a sorted
// Vec.
// If owning, Vec takes ownership of the value (e.g., if char* then use
// strdup()).
void vec_push(Vec* vec, void* value);

// Returns an owning (or borrowing) copy of the given owning (or borrowing)
// Vec using the given copy function.
Vec vec_copy(const Vec* vec, void* (*cpy)(const void*));

// Moves all vec2's values to the end of vec1's values, after which vec2 is
// freed and must not be used again.
// Only callable if both vecs are compatible, i.e., they have matching
// _cmp() and _destroy() functions (so both are owners or both are
// borrowers of the same kind of objects).
// Use case: an array of Vec's each one of which is populated in its own
// thread and at the end we want to merge all the Vec's into one.
void vec_merge(Vec* vec1, Vec* vec2);

// Returns true if the two Vec's have the same values.
bool vec_equal(const Vec* vec1, const Vec* vec2);

// Returns true if the two Vec's have the same values and the same cmp
// and destroy.
bool vec_same(const Vec* vec1, const Vec* vec2);

// Returns the index where the value was found in the Vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_find(const Vec* vec, const void* value);

// Returns the last index where the value was found in the Vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_find_last(const Vec* vec, const void* value);

// Sorts the Vec in-place using the cmp function.
// See tag_test.h's tag_cmp and sx.c's str_strcmp functions for examples
// of how to create a cmp function.
void vec_sort(Vec* vec);

// Returns the index where the value was found in the Vec or
// VEC_NOT_FOUND (-1). Uses a binary search that assumes vec_sort() has
// been used.
int vec_search(const Vec* vec, const void* value);
