// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define VEC_INITIAL_CAP 32
#define VEC_NOT_FOUND -1

// index must be a variable or literal, not an expression!
#define assert_valid_index(vec, index) \
    assert(0 <= (index) && (index) < (vec)->_size && "index out of range")

#define assert_nonempty(vec) assert((vec)->_size && "empty vec");
