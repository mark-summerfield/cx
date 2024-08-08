// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx_test_util.h"
#include <stdio.h>
#include <string.h>

void check_str_eq(int n, counts_pair* counts, const char* s,
                  const char* t) {
    counts->total++;
    if (strcmp((s), (t)) != 0) {
        fprintf(stderr, "FAIL #%d: expected\n\t\"%s\", got\n\t\"%s\"\n", n,
                s, t);
    } else
        counts->ok++;
}

void check_int_eq(int n, counts_pair* counts, int a, int b) {
    counts->total++;
    if (a != b) {
        fprintf(stderr, "FAIL #%d: expected %d, got %d\n", n, a, b);
    } else
        counts->ok++;
}
