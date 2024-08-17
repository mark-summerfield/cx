// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define VEC_INITIAL_CAP 32

#define assert_valid_index(v, index) \
    assert((index) < (v)->_size && "index out of range")

#define assert_nonempty(v) assert((v)->_size && "empty vec");
