// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <assert.h>

#define VEC_NOT_FOUND -1
#define VEC_INITIAL_CAP 16

// Returns true if the Vec is empty.
#define VEC_ISEMPTY(vec) ((vec)->_size == 0)

// Returns the Vec's size.
#define VEC_SIZE(vec) ((vec)->_size)

// Returns the Vec's capacity.
#define VEC_CAP(vec) ((vec)->_cap)

// Fast unchecked access to an element in any vector.
#define VEC_GET(vec, index) (vec)->_values[(index)]

// Fast unchecked access to the first element in any nonempty vector.
#define VEC_GET_FIRST(vec) (vec)->_values[0]

// Fast unchecked access to the last element in any nonempty vector.
#define VEC_GET_LAST(vec) (vec)->_values[(vec)->_size - 1]

// index must be a variable or literal, not an expression!
#define assert_valid_index(vec, index) \
    assert(0 <= (index) && (index) < (vec)->_size && "index out of range")

#define assert_nonempty(vec) assert((vec)->_size && "empty vec");

// The initial vec cap is 0 (unless specified). On the first add,
// insert, or push on cap 0, cap goes to VEC_INITIAL_CAP, then doubles
// each time until it exceeds 1MB, then grows by 1MB each time the cap
// is reached.
#define GROW_CAP(cap) \
    (!(cap) ? VEC_INITIAL_CAP : (cap) < 1048576 ? (cap)*2 : (cap) + 1048576)
