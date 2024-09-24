// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vecs.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef uint8_t byte;

// A vector of byte values.
// All accesses via functions, but _reading_ `_values` is okay.
// See vecs.h for size and capacity macros.
typedef struct VecByte {
    int _size; // This is "end", i.e., one past the last value
    int _cap;  // The size of the allocated array
    byte* _values;
} VecByte;

// Allocates a new empty VecByte with the given capacity.
VecByte vec_byte_alloc_cap(int cap);

// Allocates a new empty VecByte with a default capacity of 0.
#define vec_byte_alloc() vec_byte_alloc_cap(0)

// Destroys the VecByte freeing its memory. The VecByte is not usable
// after this.
void vec_byte_free(VecByte* vec);

// Clears all the VecByte's values.
void vec_byte_clear(VecByte* vec);

// Returns the VecByte's byte value at position index.
// The VEC_GET() macro is faster but unchecked.
byte vec_byte_get(const VecByte* vec, int index);

// Returns the VecByte's int value at its first valid index.
// The VEC_GET_FIRST() macro is faster but unchecked.
byte vec_byte_get_first(const VecByte* vec);

// Returns the VecByte's int value at its last valid index.
// The VEC_GET_LAST() macro is faster but unchecked.
byte vec_byte_get_last(const VecByte* vec);

// Sets the VecByte's value at position index to the given byte.
void vec_byte_set(VecByte* vec, int index, byte value);

// Inserts the byte at position index and moves succeeding values
// up (right), increasing the VecByte's size (and cap if necessary): O(n).
void vec_byte_insert(VecByte* vec, int index, byte value);

// Sets the VecByte's value at position index to the given byte
// and returns the old byte value from that position.
byte vec_byte_replace(VecByte* vec, int index, byte value);

// Removes the value at the given index and closes up the gap: O(n).
void vec_byte_remove(VecByte* vec, int index);

// Returns and removes the value at the given index and closes up the
// gap: O(n).
byte vec_byte_take(VecByte* vec, int index);

// Removes and returns the last int value. Only use if
// vec.isempty() is false: O(1).
byte vec_byte_pop(VecByte* vec);

// Pushes a new byte value onto the end of the VecByte,
// increasing the VecByte's size (and cap) if necessary: O(1).
void vec_byte_push(VecByte* vec, byte value);

// Returns a deep copy of the VecByte.
VecByte vec_byte_copy(const VecByte* vec);

// Moves all vec2's values to the end of vec1's values, after which vec2 is
// freed and must not be used again.
// The two VecByte's must have the same cmp, cpy, and destroy methods.
// Use case: an array of VecByte's each one of which is populated in its own
// thread and at the end we want to merge all the VecByte's into one.
void vec_byte_merge(VecByte* vec1, VecByte* vec2);

// Returns true if the two VecByte's have the same byte values.
bool vec_byte_equal(const VecByte* vec1, const VecByte* vec2);

// Returns the index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_byte_find(const VecByte* vec, byte value);

// Returns the last index where the value was found in the vec or
// VEC_NOT_FOUND (-1). Uses a linear search.
int vec_byte_find_last(const VecByte* vec, byte value);

// Returns a string representing the vec's byte values as
// space-separated hex numbers or NULL if the vec is empty.
// The caller owns the returned string.
char* vec_byte_to_str(const VecByte* vec);

// For debugging.
void vec_byte_dump(const VecByte* vec);

// To iterate:
//      for (int i = 0; i < VEC_SIZE(vec); ++i)
//          byte value = VEC_GET(vec, i);
