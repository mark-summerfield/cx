// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <assert.h>

//% Use after malloc or realloc
#define assert_alloc(p) assert((p) && "failed to acquire memory")

#define assert_notnull(p) assert((p) && "expected non-NULL pointer")

// Used to specify that a collection owns its pointers. See also
// BORROWS.
#define OWNS true

// Used to specify that a collection does not own its pointers. See also
// OWNS.
#define BORROWS (!OWNS)
