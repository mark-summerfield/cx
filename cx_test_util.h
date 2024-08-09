#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

typedef struct counts_pair {
    int total;
    int ok;
} counts_pair;

void check_str_eq(counts_pair* counts, const char* s, const char* t);
void check_int_eq(counts_pair* counts, int a, int b);
