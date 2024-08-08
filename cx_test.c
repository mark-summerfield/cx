// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx_test_util.h"
#include "va_test.h"
#include "vec_int_test.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void va_tests(counts_pair*);

int main(int argc, char** argv) {
    bool verbose = argc > 1;
    counts_pair counts = {0, 0};
    va_tests(&counts);
    vec_int_tests(&counts, verbose);
    printf("TODO vec_str_tests(&counts, verbose);\n");
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
