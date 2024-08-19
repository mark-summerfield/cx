// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <assert.h>

typedef int SSIZE_T;

// Use after malloc or realloc
#define assert_alloc(p) assert((p) && "failed to acquire memory")

#define assert_notnull(p) assert((p) && "unexpected NULL pointer")
