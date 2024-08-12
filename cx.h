#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <assert.h>

// Use after malloc or realloc
#define assert_alloc(p) assert((p) && "failed to acquire memory")

// Use as an argument to functions that need a cmp(a, b) function
typedef int (*cmpfn)(void*, void*);
