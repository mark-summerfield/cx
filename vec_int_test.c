// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_int_test.h"
#include "vec_int.h"
#include <stdlib.h>

void vec_int_check_size_cap(counts_pair* counts, vec_int* v, size_t size,
                            size_t capacity);
void vec_int_match(counts_pair* counts, vec_int* v, char* expected);
void vec_int_same(counts_pair* counts, vec_int* v1, vec_int* v2);
void vec_int_print(vec_int* v);
void vec_str_tests(counts_pair*, bool);

void vec_int_tests(counts_pair* counts) {
    vec_int v1 = vec_int_alloc(); // default of 32
    vec_int_check_size_cap(counts, &v1, 0, 32);

    vec_int v2 = vec_int_copy(&v1);
    vec_int_check_size_cap(counts, &v1, 0, 32);

    counts->total++;
    for (int i = 1; i <= 35; ++i) {
        vec_int_push(&v1, i);
        if (i < 10)
            vec_int_push(&v2, i);
        vec_int_check_size_cap(counts, &v1, i, i <= 32 ? 32 : 64);
    }
    counts->ok++;

    vec_int_check_size_cap(counts, &v1, 35, 64);
    vec_int_check_size_cap(counts, &v2, 9, 32);
    vec_int_match(counts, &v1,
                  "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 "
                  "20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35");
    vec_int_match(counts, &v2, "1 2 3 4 5 6 7 8 9");

    for (int i = 35; i > 9; --i) {
        int v = vec_int_pop(&v1);
        check_int_eq(counts, v, i);
    }
    vec_int_match(counts, &v1, "1 2 3 4 5 6 7 8 9");
    vec_int_same(counts, &v1, &v2);

    vec_int_push(&v1, -99);
    vec_int_match(counts, &v1, "1 2 3 4 5 6 7 8 9 -99");

    vec_int_insert(&v1, 4, -555);
    vec_int_match(counts, &v1, "1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_insert(&v1, 0, 21);
    vec_int_match(counts, &v1, "21 1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_insert(&v1, 0, 17);
    vec_int_match(counts, &v1, "17 21 1 2 3 4 -555 5 6 7 8 9 -99");

    int x = vec_int_get(&v1, 0);
    check_int_eq(counts, x, 17);
    x = vec_int_get(&v1, 4);
    check_int_eq(counts, x, 6);
    x = vec_int_get_last(&v1);
    check_int_eq(counts, x, -99);

    vec_int_remove(&v1, 1);
    vec_int_match(counts, &v1, "17 1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_remove(&v1, 0);
    vec_int_match(counts, &v1, "1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_set(&v1, 2, -33);
    vec_int_match(counts, &v1, "1 2 -33 4 -555 5 6 7 8 9 -99");
    vec_int_set(&v1, 10, 10);
    vec_int_match(counts, &v1, "1 2 -33 4 -555 5 6 7 8 9 10");
    vec_int_set(&v1, 0, 0);
    vec_int_match(counts, &v1, "0 2 -33 4 -555 5 6 7 8 9 10");

    x = vec_int_replace(&v1, 4, 111);
    check_int_eq(counts, x, -555);
    vec_int_match(counts, &v1, "0 2 -33 4 111 5 6 7 8 9 10");

    vec_int_remove(&v1, 7);
    vec_int_match(counts, &v1, "0 2 -33 4 111 5 6 8 9 10");
    vec_int_remove(&v1, 7);
    vec_int_match(counts, &v1, "0 2 -33 4 111 5 6 9 10");

    x = vec_int_take(&v1, 0);
    check_int_eq(counts, x, 0);
    vec_int_match(counts, &v1, "2 -33 4 111 5 6 9 10");
    x = vec_int_take(&v1, 7);
    check_int_eq(counts, x, 10);
    vec_int_match(counts, &v1, "2 -33 4 111 5 6 9");

    vec_found_index found_index = vec_int_find(&v1, 8);
    check_bool_eq(counts, found_index.found, false);
    found_index = vec_int_find(&v1, 2);
    check_bool_eq(counts, found_index.found, true);
    check_int_eq(counts, found_index.index, 0);
    found_index = vec_int_find(&v1, 111);
    check_bool_eq(counts, found_index.found, true);
    check_int_eq(counts, found_index.index, 3);
    found_index = vec_int_find(&v1, 9);
    check_bool_eq(counts, found_index.found, true);
    check_int_eq(counts, found_index.index, 6);

    vec_int_clear(&v1);
    vec_int_check_size_cap(counts, &v1, 0, 64);
    vec_int_push(&v1, -19);
    vec_int_match(counts, &v1, "-19");
    vec_int_free(&v1);
    vec_int_check_size_cap(counts, &v1, 0, 0);
    vec_int_clear(&v2);
    vec_int_check_size_cap(counts, &v2, 0, 32);
    vec_int_free(&v2);
    vec_int_check_size_cap(counts, &v2, 0, 0);
}

void vec_int_match(counts_pair* counts, vec_int* v, char* expected) {
    char* out = vec_int_tostring(v);
    check_str_eq(counts, out, expected);
    free(out);
}

void vec_int_check_size_cap(counts_pair* counts, vec_int* v, size_t size,
                            size_t capacity) {
    counts->total++;
    if (vec_int_size(v) != size) {
        fprintf(stderr, "FAIL: vec_int_size() expected %zu, got %zu\n",
                size, vec_int_size(v));
    } else
        counts->ok++;

    counts->total++;
    if (vec_int_cap(v) != capacity) {
        fprintf(stderr, "FAIL: vec_int_cap() expected %zu, got %zu\n",
                capacity, vec_int_cap(v));
    } else
        counts->ok++;
}

void vec_int_same(counts_pair* counts, vec_int* v1, vec_int* v2) {
    counts->total++;
    if (!vec_int_equal(v1, v2)) {
        fprintf(stderr, "FAIL: vec_int_equal() expected true, got false\n");
    } else
        counts->ok++;
}
