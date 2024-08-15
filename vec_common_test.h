// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx_util_test.h"
#include "vec_common.h"

typedef struct {
    size_t index;
    bool found;
} vec_found_index;

void vec_check_found(tinfo* tinfo, const vec_found_index* i1,
                     const vec_found_index* i2);
