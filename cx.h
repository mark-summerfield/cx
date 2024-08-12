#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define assert_alloc(p) assert((p) && "failed to acquire memory")

typedef struct {
    size_t index;
    bool found;
} maybe_found_index;

typedef int (*cmpfn)(void*, void*);
