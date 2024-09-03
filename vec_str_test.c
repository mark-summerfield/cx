// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str_test.h"
#include "str.h"
#include "vec_common_test.h"
#include "vec_str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void check_join(tinfo* tinfo, const VecStr* vec, const char* sep,
                       const char* expected);
static void check_size_cap(tinfo* tinfo, const VecStr* v, int size,
                           int cap);
static void match(tinfo* tinfo, const VecStr* v, const char* expected);
static void equal(tinfo* tinfo, const VecStr* v1, const VecStr* v2);
static void merge_tests(tinfo*);
static void sort_tests(tinfo*);
static void prefix_tests(tinfo*);

const char* WORDS[] = {
    "One",  "Zulu",    "Victor", "Romeo",  "Sierra",   "Whiskey", "X-ray",
    "Two",  "India",   "Papa",   "Quebec", "Tango",    "Yankee",  "Three",
    "Golf", "Alpha",   "Delta",  "Oscar",  "Hotel",    "Four",    "Kilo",
    "Lima", "Five",    "Juliet", "Mike",   "November", "Uniform", "Six",
    "Echo", "Foxtrot", "Seven",  "Eight",  "Charlie",  "Nine",    "Bravo",
    "Ten",  "Zero"};

void vec_str_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    tinfo->tag = "merge_tests";
    merge_tests(tinfo);
    tinfo->tag = "sort_tests";
    sort_tests(tinfo);
    tinfo->tag = "prefix_tests";
    prefix_tests(tinfo);

    tinfo->tag = "vec_str_tests continued";
    if (tinfo->verbose)
        puts(tinfo->tag);
    VecStr v1 = vec_str_alloc(); // default of 0
    check_size_cap(tinfo, &v1, 0, 0);

    VecStr v2 = vec_str_copy(&v1, BORROWS);
    check_size_cap(tinfo, &v1, 0, 0);

    tinfo->total++;
    int WORD_COUNT = sizeof(WORDS) / sizeof(char*);
    for (int i = 0; i < WORD_COUNT; ++i) {
        check_size_cap(tinfo, &v1, i, i == 0 ? 0 : i <= 32 ? 32 : 64);
        vec_str_push(&v1, strdup(WORDS[i]));
        if (i < 10) {
            check_size_cap(tinfo, &v2, i, i == 0 ? 0 : 32);
            vec_str_push(&v2, strdup(WORDS[i]));
        }
    }
    tinfo->ok++;

    check_size_cap(tinfo, &v1, WORD_COUNT, 64);
    check_size_cap(tinfo, &v2, 10, 32);
    const char* V2 =
        "One|Zulu|Victor|Romeo|Sierra|Whiskey|X-ray|Two|India|Papa";
    match(tinfo, &v2, V2);
    check_join(tinfo, &v2, "|", V2);
    check_join(tinfo, &v2, NULL,
               "OneZuluVictorRomeoSierraWhiskeyX-rayTwoIndiaPapa");
    for (int i = WORD_COUNT - 1; i > 9; --i) {
        char* s = vec_str_pop(&v1);
        check_str_eq(tinfo, s, WORDS[i]);
        free(s);
    }
    match(tinfo, &v1, V2);
    check_size_cap(tinfo, &v1, 10, 64);
    check_size_cap(tinfo, &v2, 10, 32);
    equal(tinfo, &v1, &v2);

    vec_str_push(&v1, strdup("alpha"));
    const char* s1 = vec_str_get_last(&v1);
    check_str_eq(tinfo, s1, "alpha");
    match(
        tinfo, &v1,
        "One|Zulu|Victor|Romeo|Sierra|Whiskey|X-ray|Two|India|Papa|alpha");

    vec_str_insert(&v1, 4, strdup("beta"));
    const char* s2 = vec_str_get(&v1, 4);
    check_str_eq(tinfo, s2, "beta");
    match(tinfo, &v1,
          "One|Zulu|Victor|Romeo|beta|Sierra|Whiskey|X-ray|Two|"
          "India|Papa|alpha");

    const char* s0 = vec_str_get(&v1, 0);
    check_str_eq(tinfo, s0, "One");
    s0 = vec_str_get_first(&v1);
    check_str_eq(tinfo, s0, "One");
    const char* s5 = vec_str_get(&v1, 5);
    check_str_eq(tinfo, s5, "Sierra");
    const char* s6 = vec_str_get(&v1, 6);
    check_str_eq(tinfo, s6, "Whiskey");

    vec_str_set(&v1, 6, strdup("gamma"));
    const char* s6a = vec_str_get(&v1, 6);
    check_str_eq(tinfo, s6a, "gamma");
    match(tinfo, &v1,
          "One|Zulu|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
          "India|Papa|alpha");

    vec_str_set(&v1, 0, strdup("A0"));
    const char* s0a = vec_str_get(&v1, 0);
    check_str_eq(tinfo, s0a, "A0");
    match(tinfo, &v1,
          "A0|Zulu|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
          "India|Papa|alpha");

    vec_str_insert(&v1, 2, strdup("B2"));
    const char* s2a = vec_str_get(&v1, 2);
    check_str_eq(tinfo, s2a, "B2");
    match(tinfo, &v1,
          "A0|Zulu|B2|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
          "India|Papa|alpha");

    vec_str_remove(&v1, 4);
    match(tinfo, &v1,
          "A0|Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
          "India|Papa|alpha");

    vec_str_remove(&v1, 0);
    match(tinfo, &v1,
          "Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
          "India|Papa|alpha");

    vec_str_remove(&v1, vec_str_size(&v1) - 1);
    match(tinfo, &v1,
          "Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
          "India|Papa");

    vec_str_replace(&v1, 3, strdup("Hairy"));
    match(tinfo, &v1,
          "Zulu|B2|Victor|Hairy|Sierra|gamma|X-ray|Two|"
          "India|Papa");
    char* x = vec_str_take(&v1, 1);
    check_str_eq(tinfo, x, "B2");
    free(x);
    match(tinfo, &v1,
          "Zulu|Victor|Hairy|Sierra|gamma|X-ray|Two|India|Papa");

    int index;
    index = vec_str_find(&v1, "Two");
    check_found(tinfo, index, 6);
    index = vec_str_find(&v1, "two");
    check_found(tinfo, index, VEC_NOT_FOUND);
    index = vec_str_find(&v1, "Papa");
    check_found(tinfo, index, 8);
    index = vec_str_find(&v1, "Zulu");
    check_found(tinfo, index, 0);
    index = vec_str_find_last(&v1, "two");
    check_found(tinfo, index, VEC_NOT_FOUND);
    index = vec_str_find_last(&v1, "Zulu");
    check_found(tinfo, index, 0);
    index = vec_str_find_last(&v1, "Two");
    check_found(tinfo, index, 6);

    vec_str_clear(&v1);
    check_size_cap(tinfo, &v1, 0, 64);
    vec_str_push(&v1, strdup("more"));
    const char* more = vec_str_get(&v1, 0);
    check_str_eq(tinfo, more, "more");
    match(tinfo, &v1, "more");
    check_size_cap(tinfo, &v1, 1, 64);
    vec_str_free(&v1);
    check_size_cap(tinfo, &v1, 0, 0);
    vec_str_clear(&v2);
    check_size_cap(tinfo, &v2, 0, 32);
    vec_str_free(&v2);
    check_size_cap(tinfo, &v2, 0, 0);

    VecStr v4 = vec_str_alloc_split("one\ttwo\tthree\tfour\tfive", "\t");
    match(tinfo, &v4, "one|two|three|four|five");
    vec_str_free(&v4);
    VecStr v5 =
        vec_str_alloc_split("oneSEPtwoSEPthreeSEPfourSEPfiveSEPsix", "SEP");
    match(tinfo, &v5, "one|two|three|four|five|six");
    vec_str_free(&v5);
}

static void merge_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    VecStr v1 = vec_str_alloc_custom(7, OWNS);
    check_size_cap(tinfo, &v1, 0, 7);
    check_join(tinfo, &v1, ":", NULL);
    check_join(tinfo, &v1, NULL, NULL);
    vec_str_push(&v1, strdup("one"));
    check_join(tinfo, &v1, ":", "one");
    check_join(tinfo, &v1, NULL, "one");
    vec_str_push(&v1, strdup("two"));
    check_join(tinfo, &v1, ":", "one:two");
    check_join(tinfo, &v1, NULL, "onetwo");
    vec_str_push(&v1, strdup("three"));
    check_join(tinfo, &v1, ":", "one:two:three");
    check_join(tinfo, &v1, NULL, "onetwothree");
    vec_str_push(&v1, strdup("four"));
    check_join(tinfo, &v1, ":", "one:two:three:four");
    check_join(tinfo, &v1, NULL, "onetwothreefour");
    vec_str_push(&v1, strdup("five"));
    check_join(tinfo, &v1, ":", "one:two:three:four:five");
    check_join(tinfo, &v1, NULL, "onetwothreefourfive");
    check_size_cap(tinfo, &v1, 5, 7);
    match(tinfo, &v1, "one|two|three|four|five");

    VecStr v2 =
        vec_str_alloc_split("six\tseven\teight\tnine\tten\televen", "\t");
    match(tinfo, &v2, "six|seven|eight|nine|ten|eleven");
    check_size_cap(tinfo, &v2, 6, 32);

    vec_str_merge(&v1, &v2);
    match(tinfo, &v1,
          "one|two|three|four|five|six|seven|eight|nine|ten|eleven");
    check_size_cap(tinfo, &v1, 11, 11);
    check_size_cap(tinfo, &v2, 0, 0);

    // v2 already freed by merge
    vec_str_free(&v1);
}

static void sort_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    VecStr v1 = vec_str_alloc_split(
        "Zulu|Victor|Hairy|Sierra|gamma|X-ray|Two|India|Papa", "|");
    match(tinfo, &v1,
          "Zulu|Victor|Hairy|Sierra|gamma|X-ray|Two|India|Papa");

    int index;
    index = vec_str_find(&v1, "Zulu");
    check_found(tinfo, index, 0);
    index = vec_str_find(&v1, "Papa");
    check_found(tinfo, index, 8);
    index = vec_str_find(&v1, "Sierra");
    check_found(tinfo, index, 3);
    index = vec_str_find(&v1, "Sierrb");
    check_found(tinfo, index, VEC_NOT_FOUND);

    vec_str_sort(&v1);
    match(tinfo, &v1,
          "Hairy|India|Papa|Sierra|Two|Victor|X-ray|Zulu|gamma");
    vec_str_push(&v1, strdup("Alpha"));
    vec_str_push(&v1, strdup("kilo"));
    match(tinfo, &v1,
          "Hairy|India|Papa|Sierra|Two|Victor|X-ray|Zulu|gamma|Alpha|kilo");
    vec_str_sort(&v1);
    match(tinfo, &v1,
          "Alpha|Hairy|India|Papa|Sierra|Two|Victor|X-ray|Zulu|gamma|kilo");

    index = vec_str_find(&v1, "Zulu");
    check_found(tinfo, index, 8);
    index = vec_str_search(&v1, "Zulu");
    check_found(tinfo, index, 8);

    index = vec_str_find(&v1, "Papa");
    check_found(tinfo, index, 3);
    index = vec_str_search(&v1, "Papa");
    check_found(tinfo, index, 3);

    index = vec_str_find(&v1, "Sierra");
    check_found(tinfo, index, 4);
    index = vec_str_search(&v1, "Sierra");
    check_found(tinfo, index, 4);

    index = vec_str_find(&v1, "Sierrb");
    check_found(tinfo, index, VEC_NOT_FOUND);
    index = vec_str_search(&v1, "Sierrb");
    check_found(tinfo, index, VEC_NOT_FOUND);

    index = vec_str_find(&v1, "Alpha");
    check_found(tinfo, index, 0);
    index = vec_str_search(&v1, "Alpha");
    check_found(tinfo, index, 0);

    index = vec_str_find(&v1, "kilo");
    check_found(tinfo, index, 10);
    index = vec_str_search(&v1, "kilo");
    check_found(tinfo, index, 10);

    match(tinfo, &v1,
          "Alpha|Hairy|India|Papa|Sierra|Two|Victor|X-ray|Zulu|gamma|kilo");
    vec_str_add(&v1, strdup("Aardvark"));
    match(tinfo, &v1,
          "Aardvark|Alpha|Hairy|India|Papa|Sierra|Two|Victor|X-ray|"
          "Zulu|gamma|kilo");
    vec_str_add(&v1, strdup("limbo"));
    match(tinfo, &v1,
          "Aardvark|Alpha|Hairy|India|Papa|Sierra|Two|Victor|X-ray|"
          "Zulu|gamma|kilo|limbo");
    vec_str_add(&v1, strdup("Oscar"));
    match(tinfo, &v1,
          "Aardvark|Alpha|Hairy|India|Oscar|Papa|Sierra|Two|Victor|"
          "X-ray|Zulu|gamma|kilo|limbo");
    vec_str_add(&v1, strdup("Oscar"));
    match(tinfo, &v1,
          "Aardvark|Alpha|Hairy|India|Oscar|Oscar|Papa|Sierra|Two|"
          "Victor|X-ray|Zulu|gamma|kilo|limbo");

    VecStr v2 =
        vec_str_alloc_split("Delta|bravo|Charlie|Bravo|delta|Alpha", "|");
    match(tinfo, &v2, "Delta|bravo|Charlie|Bravo|delta|Alpha");
    vec_str_casesort(&v2);
    match(tinfo, &v2, "Alpha|bravo|Bravo|Charlie|Delta|delta");

    index = vec_str_find(&v2, "Sierrb");
    check_found(tinfo, index, VEC_NOT_FOUND);
    index = vec_str_search(&v2, "Sierrb");
    check_found(tinfo, index, VEC_NOT_FOUND);

    index = vec_str_find(&v2, "Alpha");
    check_found(tinfo, index, 0);
    index = vec_str_search(&v2, "Alpha");
    check_found(tinfo, index, 0);

    index = vec_str_find(&v2, "Bravo");
    check_found(tinfo, index, 2);
    index = vec_str_casesearch(&v2, "Bravo");
    check_found(tinfo, index, 1);

    index = vec_str_find(&v2, "delta");
    check_found(tinfo, index, 5);
    index = vec_str_casesearch(&v2, "delta");
    check_found(tinfo, index, 5);

    vec_str_free(&v2);
    vec_str_free(&v1);
}

static void prefix_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    VecStr v1 = vec_str_alloc_custom(3, BORROWS);
    vec_str_push(&v1, "fan");
    vec_str_push(&v1, "fate");
    vec_str_push(&v1, "fame");
    char* prefix = vec_str_longest_common_prefix(&v1);
    check_str_eq(tinfo, prefix, "fa");
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "elefan");
    vec_str_push(&v1, "elefate");
    vec_str_push(&v1, "elefame");
    vec_str_push(&v1, "elefa");
    prefix = vec_str_longest_common_prefix(&v1);
    check_str_eq(tinfo, prefix, "elefa");
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "bat");
    vec_str_push(&v1, "vat");
    vec_str_push(&v1, "cat");
    prefix = vec_str_longest_common_prefix(&v1);
    check_bool_eq(tinfo, false, prefix); // i.e., prefix == NULL
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "/home/mark/app/go/gong");
    vec_str_push(&v1, "/home/mark/app/py/accelhints");
    vec_str_push(&v1, "/home/mark/app/rs");
    prefix = vec_str_longest_common_path(&v1);
    check_str_eq(tinfo, prefix, "/home/mark/app");
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "/users/mark/app/go/gong");
    vec_str_push(&v1, "/Users/mark/app/py/accelhints");
    vec_str_push(&v1, "/home/mark/app/rs");
    prefix = vec_str_longest_common_path(&v1);
    check_str_eq(tinfo, prefix, "/");
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "mark/app/go/gong");
    vec_str_push(&v1, "mark/app/py/accelhints");
    vec_str_push(&v1, "mark/app/rs");
    vec_str_push(&v1, "mark/app/rsc");
    prefix = vec_str_longest_common_path(&v1);
    check_str_eq(tinfo, prefix, "mark/app");
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "mark/app/go/gong");
    vec_str_push(&v1, "andrea/app/py/accelhints");
    prefix = vec_str_longest_common_path(&v1);
    check_bool_eq(tinfo, false, prefix); // i.e., prefix == NULL
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "/home/mark/app/go/gong");
    vec_str_push(&v1, "/home/mark/apps/bin");
    vec_str_push(&v1, "/home/mark/apps/py/accelhints");
    vec_str_push(&v1, "/home/mark/app/rs");
    prefix = vec_str_longest_common_path(&v1);
    check_str_eq(tinfo, prefix, "/home/mark");
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "/home/mark/bin/checksum");
    vec_str_push(&v1, "/home/mark/bin/checkkey");
    prefix = vec_str_longest_common_prefix(&v1);
    check_str_eq(tinfo, prefix, "/home/mark/bin/check");
    free(prefix);
    prefix = vec_str_longest_common_path(&v1);
    check_str_eq(tinfo, prefix, "/home/mark/bin");
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "/home/marm/bin/checksum");
    vec_str_push(&v1, "/home/mark/bin/checkkey");
    prefix = vec_str_longest_common_prefix(&v1);
    check_str_eq(tinfo, prefix, "/home/mar");
    free(prefix);
    prefix = vec_str_longest_common_path(&v1);
    check_str_eq(tinfo, prefix, "/home");
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "/home/marm/bin/checksum");
    vec_str_push(&v1, "/homer/mark/bin/checkkey");
    prefix = vec_str_longest_common_prefix(&v1);
    check_str_eq(tinfo, prefix, "/home");
    free(prefix);
    prefix = vec_str_longest_common_path(&v1);
    check_str_eq(tinfo, prefix, "/");
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "/home/page");
    vec_str_push(&v1, "/homeric/poem");
    prefix = vec_str_longest_common_path(&v1);
    check_str_eq(tinfo, prefix, "/");
    free(prefix);
    vec_str_clear(&v1);

    vec_str_push(&v1, "home");
    vec_str_push(&v1, "homeric");
    prefix = vec_str_longest_common_prefix(&v1);
    check_str_eq(tinfo, prefix, "home");
    free(prefix);
    prefix = vec_str_longest_common_path(&v1);
    check_bool_eq(tinfo, false, prefix); // i.e., prefix == NULL
    free(prefix);
    vec_str_clear(&v1);

    vec_str_free(&v1);
}

static void match(tinfo* tinfo, const VecStr* v, const char* expected) {
    char* out = vec_str_join(v, "|");
    check_str_eq(tinfo, out, expected);
    free(out);
}

static void check_size_cap(tinfo* tinfo, const VecStr* v, int size,
                           int cap) {
    tinfo->total++;
    if (vec_str_size(v) != size) {
        fprintf(stderr, "FAIL: %s vec_str_size() expected %d != %d\n",
                tinfo->tag, size, vec_str_size(v));
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_str_isempty(v) != (size == 0)) {
        fprintf(stderr,
                "FAIL: %s vec_stry_isempty() expected %s != %s size=%d\n",
                tinfo->tag, bool_to_str(size == 0),
                bool_to_str(vec_str_isempty(v)), size);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_str_cap(v) != cap) {
        fprintf(stderr, "FAIL: %s vec_str_cap() expected %d != %d\n",
                tinfo->tag, cap, vec_str_cap(v));
    } else
        tinfo->ok++;
}

static void equal(tinfo* tinfo, const VecStr* v1, const VecStr* v2) {
    tinfo->total++;
    if (!vec_str_equal(v1, v2)) {
        fprintf(stderr, "FAIL: %s vec_str_equal() expected true != false\n",
                tinfo->tag);
    } else
        tinfo->ok++;
}

static void check_join(tinfo* tinfo, const VecStr* vec, const char* sep,
                       const char* expected) {
    tinfo->total++;
    char* actual = vec_str_join(vec, sep);
    if (!vec_str_size(vec) && !actual && !expected)
        tinfo->ok++;
    else if (actual && expected && strcmp(actual, expected))
        fprintf(stderr,
                "FAIL: %s vec_str_join() expected\n\"%s\" !=\n\"%s\"\n",
                tinfo->tag, expected, actual);
    else
        tinfo->ok++;
    free(actual);
}
