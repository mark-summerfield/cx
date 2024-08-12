#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define VEC_INITIAL_SIZE 32

#define assert_valid_index(v, index) \
    assert((index) < (v)->_size && "index out of range")

// Use to return a size_t index and true or 0 and false
typedef struct {
    size_t index;
    bool found;
} vec_found_index;

typedef void* (*copyfn)(void*);
