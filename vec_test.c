// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_test.h"
#include "sx.h"
#include "tag_test.h"
#include "vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Woverride-init"
#pragma GCC diagnostic push

void vec_check_size_cap(tinfo* tinfo, const vec* v, size_t size,
                        size_t capacity);
void vec_match(tinfo* tinfo, const vec* v, const char* expected);
void vec_same(tinfo* tinfo, const vec* v1, const vec* v2);
void vec_print(const vec* v);
void vec_tests(tinfo*);
void vec_merge_tests(tinfo*);
void vec_sort_tests(tinfo*);

void vec_tests(tinfo* tinfo) {
    /*
    vec_merge_tests(tinfo);
    vec_sort_tests(tinfo);

    vec v1 = vec_alloc(); // default of 32
    vec_check_size_cap(tinfo, &v1, 0, 32);

    vec v2 = vec_copy(&v1);
    vec_check_size_cap(tinfo, &v1, 0, 32);

    tinfo->total++;
    int WORD_COUNT = sizeof(WORDS) / sizeof(char*);
    for (int i = 0; i < WORD_COUNT; ++i) {
        vec_check_size_cap(tinfo, &v1, i, i <= 32 ? 32 : 64);
        vec_push(&v1, strdup(WORDS[i]));
        if (i < 10) {
            vec_check_size_cap(tinfo, &v2, i, 32);
            vec_push(&v2, strdup(WORDS[i]));
        }
    }
    tinfo->ok++;

    vec_check_size_cap(tinfo, &v1, WORD_COUNT, 64);
    vec_check_size_cap(tinfo, &v2, 10, 32);
    const char* V2 =
        "One|Zulu|Victor|Romeo|Sierra|Whiskey|X-ray|Two|India|Papa";
    vec_match(tinfo, &v2, V2);

    for (int i = WORD_COUNT - 1; i > 9; --i) {
        char* s = vec_pop(&v1);
        check_eq(tinfo, s, WORDS[i]);
        free(s);
    }
    vec_match(tinfo, &v1, V2);
    vec_check_size_cap(tinfo, &v1, 10, 64);
    vec_check_size_cap(tinfo, &v2, 10, 32);
    vec_same(tinfo, &v1, &v2);

    vec_push(&v1, strdup("alpha"));
    const char* s1 = vec_get_last(&v1);
    check_eq(tinfo, s1, "alpha");
    vec_match(
        tinfo, &v1,
        "One|Zulu|Victor|Romeo|Sierra|Whiskey|X-ray|Two|India|Papa|alpha");

    vec_insert(&v1, 4, strdup("beta"));
    const char* s2 = vec_get(&v1, 4);
    check_eq(tinfo, s2, "beta");
    vec_match(tinfo, &v1,
                  "One|Zulu|Victor|Romeo|beta|Sierra|Whiskey|X-ray|Two|"
                  "India|Papa|alpha");

    const char* s0 = vec_get(&v1, 0);
    check_eq(tinfo, s0, "One");
    const char* s5 = vec_get(&v1, 5);
    check_eq(tinfo, s5, "Sierra");
    const char* s6 = vec_get(&v1, 6);
    check_eq(tinfo, s6, "Whiskey");

    vec_set(&v1, 6, strdup("gamma"));
    const char* s6a = vec_get(&v1, 6);
    check_eq(tinfo, s6a, "gamma");
    vec_match(tinfo, &v1,
                  "One|Zulu|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_set(&v1, 0, strdup("A0"));
    const char* s0a = vec_get(&v1, 0);
    check_eq(tinfo, s0a, "A0");
    vec_match(tinfo, &v1,
                  "A0|Zulu|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_insert(&v1, 2, strdup("B2"));
    const char* s2a = vec_get(&v1, 2);
    check_eq(tinfo, s2a, "B2");
    vec_match(tinfo, &v1,
                  "A0|Zulu|B2|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_remove(&v1, 4);
    vec_match(tinfo, &v1,
                  "A0|Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_remove(&v1, 0);
    vec_match(tinfo, &v1,
                  "Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_remove(&v1, vec_size(&v1) - 1);
    vec_match(tinfo, &v1,
                  "Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa");

    vec_replace(&v1, 3, strdup("Hairy"));
    vec_match(tinfo, &v1,
                  "Zulu|B2|Victor|Hairy|Sierra|gamma|X-ray|Two|"
                  "India|Papa");
    char* x = vec_take(&v1, 1);
    check_eq(tinfo, x, "B2");
    free(x);
    vec_match(tinfo, &v1,
                  "Zulu|Victor|Hairy|Sierra|gamma|X-ray|Two|India|Papa");

    vec_found_index found_index = vec_find(&v1, "Two");
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 6);
    found_index = vec_find(&v1, "two");
    check_bool_eq(tinfo, found_index.found, false);
    found_index = vec_find(&v1, "Papa");
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 8);
    found_index = vec_find(&v1, "Zulu");
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 0);

    vec_clear(&v1);
    vec_check_size_cap(tinfo, &v1, 0, 64);
    vec_push(&v1, strdup("more"));
    const char* more = vec_get(&v1, 0);
    check_eq(tinfo, more, "more");
    vec_match(tinfo, &v1, "more");
    vec_check_size_cap(tinfo, &v1, 1, 64);
    vec_free(&v1);
    vec_check_size_cap(tinfo, &v1, 0, 0);
    vec_clear(&v2);
    vec_check_size_cap(tinfo, &v2, 0, 32);
    vec_free(&v2);
    vec_check_size_cap(tinfo, &v2, 0, 0);

    vec v4 = vec_alloc_split("one\ttwo\tthree\tfour\tfive", "\t");
    vec_match(tinfo, &v4, "one|two|three|four|five");
    vec_free(&v4);
    vec v5 =
        vec_alloc_split("oneSEPtwoSEPthreeSEPfourSEPfiveSEPsix", "SEP");
    vec_match(tinfo, &v5, "one|two|three|four|five|six");
    vec_free(&v5);
    */
}

void vec_merge_tests(tinfo* tinfo) {
    vec v1 = vec_alloc(.cap = 7);
    vec_check_size_cap(tinfo, &v1, 0, 7);
    vec_push(&v1, strdup("one"));
    vec_push(&v1, strdup("two"));
    vec_push(&v1, strdup("three"));
    vec_push(&v1, strdup("four"));
    vec_push(&v1, strdup("five"));
    vec_check_size_cap(tinfo, &v1, 5, 7);
    vec_match(tinfo, &v1, "one|two|three|four|five");

    /*
    vec v2 = vec_alloc_split("six\tseven\teight\tnine\tten\televen", "\t");
    vec_match(tinfo, &v2, "six|seven|eight|nine|ten|eleven");
    vec_check_size_cap(tinfo, &v2, 6, 32);

    vec_merge(&v1, &v2);
    vec_match(tinfo, &v1,
              "one|two|three|four|five|six|seven|eight|nine|ten|eleven");
    vec_check_size_cap(tinfo, &v1, 11, 11);
    vec_check_size_cap(tinfo, &v2, 0, 0);
    */

    // v2 already freed
    vec_free(&v1);
}

void vec_sort_tests(tinfo* tinfo) {
    /*
    vec v1 = vec_alloc_split(
        "Zulu|Victor|Hairy|Sierra|gamma|X-ray|Two|India|Papa", "|");
    vec_match(tinfo, &v1,
              "Zulu|Victor|Hairy|Sierra|gamma|X-ray|Two|India|Papa");

    vec_found_index found_index = vec_find(&v1, "Zulu");
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 0);
    found_index = vec_find(&v1, "Papa");
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 8);
    found_index = vec_find(&v1, "Sierra");
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 3);
    found_index = vec_find(&v1, "sierra");
    check_bool_eq(tinfo, found_index.found, false);

    vec_sort(&v1);
    vec_match(tinfo, &v1,
              "Hairy|India|Papa|Sierra|Two|Victor|X-ray|Zulu|gamma");
    vec_push(&v1, strdup("Alpha"));
    vec_push(&v1, strdup("kilo"));
    vec_match(
        tinfo, &v1,
        "Hairy|India|Papa|Sierra|Two|Victor|X-ray|Zulu|gamma|Alpha|kilo");
    vec_sort(&v1);
    vec_match(
        tinfo, &v1,
        "Alpha|Hairy|India|Papa|Sierra|Two|Victor|X-ray|Zulu|gamma|kilo");

    vec_found_index search_index;
    found_index = vec_find(&v1, "Zulu");
    search_index = vec_search(&v1, "Zulu");
    vec_check_found(tinfo, &found_index, &search_index);
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 8);
    found_index = vec_find(&v1, "Papa");
    search_index = vec_search(&v1, "Papa");
    vec_check_found(tinfo, &found_index, &search_index);
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 3);
    found_index = vec_find(&v1, "Sierra");
    search_index = vec_search(&v1, "Sierra");
    vec_check_found(tinfo, &found_index, &search_index);
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 4);
    found_index = vec_find(&v1, "Sierrb");
    search_index = vec_search(&v1, "Sierrb");
    vec_check_found(tinfo, &found_index, &search_index);
    check_bool_eq(tinfo, found_index.found, false);
    found_index = vec_find(&v1, "Alpha");
    search_index = vec_search(&v1, "Alpha");
    vec_check_found(tinfo, &found_index, &search_index);
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 0);
    found_index = vec_find(&v1, "kilo");
    search_index = vec_search(&v1, "kilo");
    vec_check_found(tinfo, &found_index, &search_index);
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 10);

    vec_free(&v1);
    */
}

void vec_match(tinfo* tinfo, const vec* v, const char* expected) {
    /*
    char* out = vec_join(v, "|");
    check_eq(tinfo, out, expected);
    free(out);
    */
}

void vec_check_size_cap(tinfo* tinfo, const vec* v, size_t size,
                        size_t capacity) {
    tinfo->total++;
    if (vec_size(v) != size) {
        fprintf(stderr, "FAIL: %s vec_size() expected %zu, got %zu\n",
                tinfo->tag, size, vec_size(v));
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_isempty(v) != (size == 0)) {
        fprintf(stderr,
                "FAIL: %s vecy_isempty() expected %s, got %s size=%zu\n",
                tinfo->tag, bool_to_str(size == 0),
                bool_to_str(vec_isempty(v)), size);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_cap(v) != capacity) {
        fprintf(stderr, "FAIL: %s vec_cap() expected %zu, got %zu\n",
                tinfo->tag, capacity, vec_cap(v));
    } else
        tinfo->ok++;
}

void vec_same(tinfo* tinfo, const vec* v1, const vec* v2) {
    tinfo->total++;
    if (!vec_equal(v1, v2)) {
        fprintf(stderr, "FAIL: %s vec_equal() expected true, got false\n",
                tinfo->tag);
    } else
        tinfo->ok++;
}

#pragma GCC diagnostic pop
