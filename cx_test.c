// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx_util_test.h"
#include "sx_test.h"
#include "va_test.h"
#include "vec_int_test.h"
#include "vec_str_test.h"
#include <stdio.h>

void va_tests(counts_pair*);

int main() {
    counts_pair counts = {"", 0, 0};
    counts.tag = "va_tests";
    va_tests(&counts);
    counts.tag = "sx_tests";
    sx_tests(&counts);
    counts.tag = "vec_int_tests";
    vec_int_tests(&counts);
    counts.tag = "vec_str_tests";
    vec_str_tests(&counts);
    printf("%s %d/%d\n", (counts.ok == counts.total) ? "OK" : "FAIL",
           counts.ok, counts.total);
}

void va_tests(counts_pair* counts) {
    counts->total++;
    int i = va_test(5);
    if (i != 8) {
        fprintf(stderr, "FAIL: va_test() expecte 8 go %d\n", i);
    } else
        counts->ok++;
    counts->total++;
    i = va_test(2, 9);
    if (i != 11) {
        fprintf(stderr, "FAIL: va_test() expecte 11 go %d\n", i);
    } else
        counts->ok++;
}
