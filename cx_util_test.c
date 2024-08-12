// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx_util_test.h"
#include "cx.h"
#include <stdio.h>
#include <string.h>

void check_str_eq(counts_pair* counts, const char* s, const char* t) {
    counts->total++;
    if (strcmp((s), (t)) != 0) {
        fprintf(stderr, "FAIL: expected\n\t\"%s\", got\n\t\"%s\"\n", s, t);
    } else
        counts->ok++;
}

void check_int_eq(counts_pair* counts, int a, int b) {
    counts->total++;
    if (a != b) {
        fprintf(stderr, "FAIL: expected %d, got %d\n", a, b);
    } else
        counts->ok++;
}

void check_bool_eq(counts_pair* counts, bool actual, bool expected) {
    counts->total++;
    if (actual != expected)
        fprintf(stderr, "FAIL: expected %s, got %s\n",
                bool_to_str(expected), bool_to_str(actual));
    else
        counts->ok++;
}
