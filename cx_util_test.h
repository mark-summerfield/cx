#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <stdbool.h>

typedef struct {
    int total;
    int ok;
} counts_pair;

void check_str_eq(counts_pair* counts, const char* s, const char* t);
void check_int_eq(counts_pair* counts, int a, int b);
void check_bool_eq(counts_pair* counts, bool actual, bool expected);
