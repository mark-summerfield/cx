#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec.h"
#include <stdbool.h>
#include <stdint.h>

bool vec_uint_eq(void* a, void* b) { return (uintptr_t)a == (uintptr_t)b; }

#define vec_uint_alloc(...)                                                \
    vec_alloc_((vec_alloc_args){.cap = 8, __VA_ARGS__})

#define vec_uint_get(v, index) (uintptr_t) vec_get((v), (index))

#define vec_uint_set(v, index, value)                                      \
    vec_set((v), (index), (void*)(uintptr_t)(value))

#define vec_uint_replace(v, index, value)                                  \
    (uintptr_t) vec_replace((v), (index), (void*)(uintptr_t)(value))

#define vec_uint_take(v, index) (uintptr_t) vec_take((v), (index))

#define vec_uint_push(v, value) vec_push((v), (void*)(uintptr_t)(value))

#define vec_uint_pop(v) (uintptr_t) vec_pop((v))
