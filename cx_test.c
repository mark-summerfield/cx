// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "va_test.h"
#include "vec_int.h"
#include "vec_str.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct counts_pair {
    int total;
    int ok;
} counts_pair;

void va_tests(counts_pair*);
void vec_int_tests(counts_pair*);
void vec_str_tests(counts_pair*);

int main() {
    counts_pair counts = {0, 0};
    va_tests(&counts);
    vec_int_tests(&counts);
    vec_str_tests(&counts);
    printf("%s %d/%d\n", (counts.ok == counts.total) ? "OK" : "FAIL",
           counts.ok, counts.total);
}

void va_tests(counts_pair* counts) {
    counts->total++;
    int j = opt_test();
    if (j != 14) {
        fprintf(stderr, "FAIL: opt_test() expecte 14 go %d\n", j);
    } else
        counts->ok++;
    counts->total++;
    j = opt_test(.i = 19);
    if (j != 22) {
        fprintf(stderr, "FAIL: opt_test() expecte 22 go %d\n", j);
    } else
        counts->ok++;
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

void vec_int_tests(counts_pair* counts) {
    vec_int v1 = vec_int_alloc(5);
    counts->total++;
    if (vec_size(&v1)) {
        fprintf(stderr, "FAIL: vec_int() expected size 0, got %ld\n",
                vec_size(&v1));
    } else
        counts->ok++;
    for (int i = 1; i <= 10; i++) {
        // printf("vec_int_push %d\n", i);
        vec_int_push(&v1, i);
    }
    // TODO
}

void vec_str_tests(counts_pair* counts) {
    // counts->total++;
    //  TODO
}
