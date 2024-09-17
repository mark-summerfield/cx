// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <assert.h>

#define VEC_NOT_FOUND -1

// index must be a variable or literal, not an expression!
#define assert_valid_index(vec, index) \
    assert(0 <= (index) && (index) < (vec)->_size && "index out of range")

#define assert_nonempty(vec) assert((vec)->_size && "empty vec");

// The initial vec cap is 0 (unless specified). On the first add,
// insert, or push on cap 0, cap goes to 32, then doubles each time
// until it exceeds 1MB, then grows by 1MB each time the cap is reached.
#define GROW_CAP(cap) \
    (!(cap) ? 32 : (cap) < 1048576 ? (cap)*2 : (cap) + 1048576)
