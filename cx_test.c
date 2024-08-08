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
void vec_int_tests(counts_pair*, bool);
void vec_int_check_size_cap(int n, counts_pair* counts, vec_int* v,
                            size_t size, size_t capacity, bool verbose);
void vec_str_tests(counts_pair*, bool);

int main(int argc, char** argv) {
    bool verbose = argc > 1;
    counts_pair counts = {0, 0};
    va_tests(&counts);
    vec_int_tests(&counts, verbose);
    vec_str_tests(&counts, verbose);
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

void vec_int_tests(counts_pair* counts, bool verbose) {
    vec_int v1 = vec_int_alloc(); // default of 32
    vec_int_check_size_cap(1, counts, &v1, 0, 32, verbose);

    counts->total++;
    for (int i = 1; i <= 40; i++) {
        vec_int_push(&v1, i);
        vec_int_check_size_cap(2, counts, &v1, i, i <= 32 ? 32 : 64,
                               verbose);
    }
    counts->ok++;

    vec_int_check_size_cap(2, counts, &v1, 40, 64, verbose);

    // TODO

    vec_int_clear(&v1);
    vec_int_check_size_cap(3, counts, &v1, 0, 0, verbose);
}

void vec_int_check_size_cap(int n, counts_pair* counts, vec_int* v,
                            size_t size, size_t capacity, bool verbose) {
    counts->total++;
    if (vec_size(v) != size) {
        fprintf(stderr, "FAIL #%d: vec_int_size() expected %zu, got %zu\n",
                n, size, vec_size(v));
    } else
        counts->ok++;

    counts->total++;
    if (vec_cap(v) != capacity) {
        fprintf(stderr, "FAIL #%d: vec_int_cap() expected %zu, got %zu\n",
                n, capacity, vec_cap(v));
    } else
        counts->ok++;
}

void vec_str_tests(counts_pair* counts, bool verbose) {
    // counts->total++;
    //  TODO
}
