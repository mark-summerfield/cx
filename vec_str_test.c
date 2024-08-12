// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str_test.h"
#include "vec_str.h"
#include <stdlib.h>
#include <string.h>

void vec_str_check_size_cap(counts_pair* counts, vec* v, size_t size,
                            size_t capacity, bool verbose);
void vec_str_match(counts_pair* counts, vec* v, char* expected);
void vec_str_same(counts_pair* counts, vec* v1, vec* v2);
void vec_str_print(vec* v);
void vec_str_tests(counts_pair*, bool);

const char* WORDS[] = {
    "One",  "Zulu",    "Victor", "Romeo",  "Sierra",   "Whiskey", "X-ray",
    "Two",  "India",   "Papa",   "Quebec", "Tango",    "Yankee",  "Three",
    "Golf", "Alpha",   "Delta",  "Oscar",  "Hotel",    "Four",    "Kilo",
    "Lima", "Five",    "Juliet", "Mike",   "November", "Uniform", "Six",
    "Echo", "Foxtrot", "Seven",  "Eight",  "Charlie",  "Nine",    "Bravo",
    "Ten",  "Zero"};

void vec_str_tests(counts_pair* counts, bool verbose) {
    vec v1 = vec_str_alloc(); // default of 32
    vec_str_check_size_cap(counts, &v1, 0, 32, verbose);

    vec v2 = vec_str_copy(&v1);
    vec_str_check_size_cap(counts, &v1, 0, 32, verbose);

    counts->total++;
    size_t count = sizeof(WORDS) / sizeof(char*);
    for (int i = 0; i < count; ++i) {
        vec_str_check_size_cap(counts, &v1, i, i <= 32 ? 32 : 64, verbose);
        vec_str_push(&v1, strdup(WORDS[i]));
        if (i < 10) {
            vec_str_check_size_cap(counts, &v2, i, 32, verbose);
            vec_str_push(&v2, strdup(WORDS[i]));
        }
    }
    counts->ok++;

    /*
    vec_str_check_size_cap(counts, &v1, 35, 64, verbose);
    vec_str_check_size_cap(counts, &v2, 9, 32, verbose);
    vec_str_match(counts, &v1,
                  "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 "
                  "20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35");
    vec_str_match(counts, &v2, "1 2 3 4 5 6 7 8 9");

    for (int i = 35; i > 9; --i) {
        int v = vec_str_pop(&v1);
        check_int_eq(counts, v, i);
    }
    vec_str_match(counts, &v1, "1 2 3 4 5 6 7 8 9");
    vec_str_same(counts, &v1, &v2);

    vec_str_push(&v1, -99);
    vec_str_match(counts, &v1, "1 2 3 4 5 6 7 8 9 -99");

    vec_str_insert(&v1, 4, -555);
    vec_str_match(counts, &v1, "1 2 3 4 -555 5 6 7 8 9 -99");

    int x = vec_str_get(&v1, 0);
    check_int_eq(counts, x, 1);
    x = vec_str_get(&v1, 4);
    check_int_eq(counts, x, -555);
    x = vec_str_get_last(&v1);
    check_int_eq(counts, x, -99);

    vec_str_set(&v1, 2, -33);
    vec_str_match(counts, &v1, "1 2 -33 4 -555 5 6 7 8 9 -99");
    vec_str_set(&v1, 10, 10);
    vec_str_match(counts, &v1, "1 2 -33 4 -555 5 6 7 8 9 10");
    vec_str_set(&v1, 0, 0);
    vec_str_match(counts, &v1, "0 2 -33 4 -555 5 6 7 8 9 10");

    x = vec_str_replace(&v1, 4, 111);
    check_int_eq(counts, x, -555);
    vec_str_match(counts, &v1, "0 2 -33 4 111 5 6 7 8 9 10");

    vec_str_remove(&v1, 7);
    vec_str_match(counts, &v1, "0 2 -33 4 111 5 6 8 9 10");
    vec_str_remove(&v1, 7);
    vec_str_match(counts, &v1, "0 2 -33 4 111 5 6 9 10");

    x = vec_str_take(&v1, 0);
    check_int_eq(counts, x, 0);
    vec_str_match(counts, &v1, "2 -33 4 111 5 6 9 10");
    x = vec_str_take(&v1, 7);
    check_int_eq(counts, x, 10);
    vec_str_match(counts, &v1, "2 -33 4 111 5 6 9");

    vec_found_index found_index = vec_str_find(&v1, 8);
    check_bool_eq(counts, found_index.found, false);
    found_index = vec_str_find(&v1, 2);
    check_bool_eq(counts, found_index.found, true);
    check_int_eq(counts, found_index.index, 0);
    found_index = vec_str_find(&v1, 111);
    check_bool_eq(counts, found_index.found, true);
    check_int_eq(counts, found_index.index, 3);
    found_index = vec_str_find(&v1, 9);
    check_bool_eq(counts, found_index.found, true);
    check_int_eq(counts, found_index.index, 6);

    vec_str_clear(&v1);
    vec_str_check_size_cap(counts, &v1, 0, 0, verbose);
    vec_str_free(&v2);
    vec_str_check_size_cap(counts, &v2, 0, 0, verbose);
    */

    vec v3 = vec_str_alloc_split("one\ttwo\tthree\tfour\tfive", "\t");
    vec_str_match(counts, &v3, "one|two|three|four|five");
    vec v4 =
        vec_str_alloc_split("oneSEPtwoSEPthreeSEPfourSEPfiveSEPsix", "SEP");
    vec_str_match(counts, &v4, "one|two|three|four|five|six");
}

void vec_str_match(counts_pair* counts, vec* v, char* expected) {
    char* out = vec_str_join(v, "|");
    check_str_eq(counts, out, expected);
    free(out);
}

void vec_str_check_size_cap(counts_pair* counts, vec* v, size_t size,
                            size_t capacity, bool verbose) {
    counts->total++;
    if (vec_str_size(v) != size) {
        fprintf(stderr, "FAIL: vec_str_size() expected %zu, got %zu\n",
                size, vec_str_size(v));
    } else
        counts->ok++;

    counts->total++;
    if (vec_str_cap(v) != capacity) {
        fprintf(stderr, "FAIL: vec_str_cap() expected %zu, got %zu\n",
                capacity, vec_str_cap(v));
    } else
        counts->ok++;
}

void vec_str_same(counts_pair* counts, vec* v1, vec* v2) {
    counts->total++;
    if (!vec_str_equal(v1, v2)) {
        fprintf(stderr, "FAIL: vec_str_equal() expected true, got false\n");
    } else
        counts->ok++;
}
