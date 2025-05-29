// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3
#pragma once

#include <assert.h>

//% Use after malloc or realloc
#define assert_alloc(p) assert((p) && "failed to acquire memory")

#define assert_notnull(p) assert((p) && "expected non-NULL pointer")

// Used to set whether a collection owns its element pointers (and must
// free them) or not.
// See, for example, deq_str_alloc(), map_str_real_alloc(), set_str_copy(),
// set_str_to_vec(), vec_str_alloc_custom(), and vec_str_copy().
typedef enum Ownership {
    Owns,    // Collection owns its element pointers and will free them
    Borrows, // Collection will not free its element pointers
} Ownership;
