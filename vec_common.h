// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define VEC_INITIAL_CAP 32
#define VEC_NOT_FOUND ((int)-1)

// index must be a variable or literal, not an expression!
#define assert_valid_index(me, index) \
    assert(0 <= (index) && (index) < (me)->_size && "index out of range")

#define assert_nonempty(me) assert((me)->_size && "empty vec");
