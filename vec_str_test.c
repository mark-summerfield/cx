// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str_test.h"
#include "sx.h"
#include "vec_str.h"
#include <stdlib.h>
#include <string.h>

void vec_str_check_size_cap(tinfo* tinfo, const vec* v, size_t size,
                            size_t capacity);
void vec_str_match(tinfo* tinfo, const vec* v, const char* expected);
void vec_str_same(tinfo* tinfo, const vec* v1, const vec* v2);
void vec_str_print(const vec* v);
void vec_str_tests(tinfo*);

const char* WORDS[] = {
    "One",  "Zulu",    "Victor", "Romeo",  "Sierra",   "Whiskey", "X-ray",
    "Two",  "India",   "Papa",   "Quebec", "Tango",    "Yankee",  "Three",
    "Golf", "Alpha",   "Delta",  "Oscar",  "Hotel",    "Four",    "Kilo",
    "Lima", "Five",    "Juliet", "Mike",   "November", "Uniform", "Six",
    "Echo", "Foxtrot", "Seven",  "Eight",  "Charlie",  "Nine",    "Bravo",
    "Ten",  "Zero"};

void vec_str_tests(tinfo* tinfo) {
    vec v1 = vec_str_alloc(); // default of 32
    vec_str_check_size_cap(tinfo, &v1, 0, 32);

    vec v2 = vec_str_copy(&v1);
    vec_str_check_size_cap(tinfo, &v1, 0, 32);

    tinfo->total++;
    int WORD_COUNT = sizeof(WORDS) / sizeof(char*);
    for (int i = 0; i < WORD_COUNT; ++i) {
        vec_str_check_size_cap(tinfo, &v1, i, i <= 32 ? 32 : 64);
        vec_str_push(&v1, strdup(WORDS[i]));
        if (i < 10) {
            vec_str_check_size_cap(tinfo, &v2, i, 32);
            vec_str_push(&v2, strdup(WORDS[i]));
        }
    }
    tinfo->ok++;

    vec_str_check_size_cap(tinfo, &v1, WORD_COUNT, 64);
    vec_str_check_size_cap(tinfo, &v2, 10, 32);
    const char* V2 =
        "One|Zulu|Victor|Romeo|Sierra|Whiskey|X-ray|Two|India|Papa";
    vec_str_match(tinfo, &v2, V2);

    for (int i = WORD_COUNT - 1; i > 9; --i) {
        char* s = vec_str_pop(&v1);
        check_str_eq(tinfo, s, WORDS[i]);
        free(s);
    }
    vec_str_match(tinfo, &v1, V2);
    vec_str_same(tinfo, &v1, &v2);

    vec_str_push(&v1, strdup("alpha"));
    const char* s1 = vec_str_get_last(&v1);
    check_str_eq(tinfo, s1, "alpha");
    vec_str_insert(&v1, 4, strdup("beta"));
    const char* s2 = vec_str_get(&v1, 4);
    check_str_eq(tinfo, s2, "beta");
    const char* s0 = vec_str_get(&v1, 0);
    check_str_eq(tinfo, s0, "One");
    const char* s5 = vec_str_get(&v1, 5);
    check_str_eq(tinfo, s5, "Sierra");
    const char* s6 = vec_str_get(&v1, 6);
    check_str_eq(tinfo, s6, "Whiskey");
    vec_str_set(&v1, 6, strdup("gamma"));
    const char* s6a = vec_str_get(&v1, 6);
    check_str_eq(tinfo, s6a, "gamma");
    vec_str_set(&v1, 0, strdup("A0"));
    const char* s0a = vec_str_get(&v1, 0);
    check_str_eq(tinfo, s0a, "A0");
    /* TODO
    vec_str_insert(&v1, 0, 21);
    vec_str_match(tinfo, &v1, "21 1 2 3 4 -555 5 6 7 8 9 -99");

    vec_str_insert(&v1, 0, 17);
    vec_str_match(tinfo, &v1, "17 21 1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int v3 = vec_int_copy(&v1);
    vec_int_check_size_cap(tinfo, &v3, vec_int_size(&v1),
                           vec_int_size(&v1));

    int x = vec_str_get(&v1, 0);
    check_int_eq(tinfo, x, 17);
    x = vec_str_get(&v1, 4);
    check_int_eq(tinfo, x, 3);
    x = vec_str_get_last(&v1);
    check_int_eq(tinfo, x, -99);

    vec_str_remove(&v1, 1);
    vec_str_match(tinfo, &v1, "17 1 2 3 4 -555 5 6 7 8 9 -99");

    vec_str_remove(&v1, 0);
    vec_str_match(tinfo, &v1, "1 2 3 4 -555 5 6 7 8 9 -99");
    x = vec_str_replace(&v1, 4, 111);
    check_int_eq(tinfo, x, -555);
    vec_str_match(tinfo, &v1, "0 2 -33 4 111 5 6 7 8 9 10");

    vec_str_remove(&v1, 7);
    vec_str_match(tinfo, &v1, "0 2 -33 4 111 5 6 8 9 10");
    vec_str_remove(&v1, 7);
    vec_str_match(tinfo, &v1, "0 2 -33 4 111 5 6 9 10");

    x = vec_str_take(&v1, 0);
    check_int_eq(tinfo, x, 0);
    vec_str_match(tinfo, &v1, "2 -33 4 111 5 6 9 10");
    x = vec_str_take(&v1, 7);
    check_int_eq(tinfo, x, 10);
    vec_str_match(tinfo, &v1, "2 -33 4 111 5 6 9");

    vec_found_index found_index = vec_str_find(&v1, 8);
    check_bool_eq(tinfo, found_index.found, false);

    found_index = vec_str_find(&v1, 2);
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 0);

    found_index = vec_str_find(&v1, 111);
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 3);

    found_index = vec_str_find(&v1, 9);
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 6);

    vec_int_match(tinfo, &v3, "17 21 1 2 3 4 -555 5 6 7 8 9 -99");
    vec_int_sort(&v3);
    vec_int_match(tinfo, &v3, "-555 -99 1 2 3 4 5 6 7 8 9 17 21");

    found_index = vec_int_search(&v3, 11);
    check_bool_eq(tinfo, found_index.found, false);

    found_index = vec_int_search(&v3, 21);
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 12);

    found_index = vec_int_search(&v3, -555);
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 0);

    found_index = vec_int_search(&v3, 7);
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 8);

    vec_str_free(&v3);
    vec_str_check_size_cap(tinfo, &v3, 0, 0);
    */

    vec_str_clear(&v1);
    vec_str_check_size_cap(tinfo, &v1, 0, 64);
    vec_str_push(&v1, strdup("more"));
    const char* more = vec_str_get(&v1, 0);
    check_str_eq(tinfo, more, "more");
    vec_str_match(tinfo, &v1, "more");
    vec_str_check_size_cap(tinfo, &v1, 1, 64);
    vec_str_free(&v1);
    vec_str_check_size_cap(tinfo, &v1, 0, 0);
    vec_str_clear(&v2);
    vec_str_check_size_cap(tinfo, &v2, 0, 32);
    vec_str_free(&v2);
    vec_str_check_size_cap(tinfo, &v2, 0, 0);

    vec v4 = vec_str_alloc_split("one\ttwo\tthree\tfour\tfive", "\t");
    vec_str_match(tinfo, &v4, "one|two|three|four|five");
    vec_str_free(&v4);
    vec v5 =
        vec_str_alloc_split("oneSEPtwoSEPthreeSEPfourSEPfiveSEPsix", "SEP");
    vec_str_match(tinfo, &v5, "one|two|three|four|five|six");
    vec_str_free(&v5);
}

void vec_str_match(tinfo* tinfo, const vec* v, const char* expected) {
    char* out = vec_str_join(v, "|");
    check_str_eq(tinfo, out, expected);
    free(out);
}

void vec_str_check_size_cap(tinfo* tinfo, const vec* v, size_t size,
                            size_t capacity) {
    tinfo->total++;
    if (vec_str_size(v) != size) {
        fprintf(stderr, "FAIL: %s vec_str_size() expected %zu, got %zu\n",
                tinfo->tag, size, vec_str_size(v));
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_str_isempty(v) != (size == 0)) {
        fprintf(
            stderr,
            "FAIL: %s vec_stry_isempty() expected %s, got %s size=%zu\n",
            tinfo->tag, bool_to_str(size == 0),
            bool_to_str(vec_str_isempty(v)), size);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_str_cap(v) != capacity) {
        fprintf(stderr, "FAIL: %s vec_str_cap() expected %zu, got %zu\n",
                tinfo->tag, capacity, vec_str_cap(v));
    } else
        tinfo->ok++;
}

void vec_str_same(tinfo* tinfo, const vec* v1, const vec* v2) {
    tinfo->total++;
    if (!vec_str_equal(v1, v2)) {
        fprintf(stderr,
                "FAIL: %s vec_str_equal() expected true, got false\n",
                tinfo->tag);
    } else
        tinfo->ok++;
}
