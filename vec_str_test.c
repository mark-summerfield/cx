// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str_test.h"
#include "sx.h"
#include "vec_common_test.h"
#include "vec_str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_join(tinfo* tinfo, const VecStr* vec, const char* sep,
                const char* expected);
void vec_str_check_size_cap(tinfo* tinfo, const VecStr* v, int size,
                            int cap);
void vec_str_match(tinfo* tinfo, const VecStr* v, const char* expected);
void vec_str_same(tinfo* tinfo, const VecStr* v1, const VecStr* v2);
void vec_str_print(const VecStr* v);
void vec_str_tests(tinfo*);
void vec_str_merge_tests(tinfo*);
void vec_str_sort_tests(tinfo*);

const char* WORDS[] = {
    "One",  "Zulu",    "Victor", "Romeo",  "Sierra",   "Whiskey", "X-ray",
    "Two",  "India",   "Papa",   "Quebec", "Tango",    "Yankee",  "Three",
    "Golf", "Alpha",   "Delta",  "Oscar",  "Hotel",    "Four",    "Kilo",
    "Lima", "Five",    "Juliet", "Mike",   "November", "Uniform", "Six",
    "Echo", "Foxtrot", "Seven",  "Eight",  "Charlie",  "Nine",    "Bravo",
    "Ten",  "Zero"};

void vec_str_tests(tinfo* tinfo) {
    vec_str_merge_tests(tinfo);
    vec_str_sort_tests(tinfo);

    VecStr v1 = vec_str_alloc(); // default of 32
    vec_str_check_size_cap(tinfo, &v1, 0, 32);

    VecStr v2 = vec_str_copy(&v1);
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
    check_join(tinfo, &v2, "|", V2);
    check_join(tinfo, &v2, NULL,
               "OneZuluVictorRomeoSierraWhiskeyX-rayTwoIndiaPapa");
    for (int i = WORD_COUNT - 1; i > 9; --i) {
        char* s = vec_str_pop(&v1);
        check_str_eq(tinfo, s, WORDS[i]);
        free(s);
    }
    vec_str_match(tinfo, &v1, V2);
    vec_str_check_size_cap(tinfo, &v1, 10, 64);
    vec_str_check_size_cap(tinfo, &v2, 10, 32);
    vec_str_same(tinfo, &v1, &v2);

    vec_str_push(&v1, strdup("alpha"));
    const char* s1 = vec_str_get_last(&v1);
    check_str_eq(tinfo, s1, "alpha");
    vec_str_match(
        tinfo, &v1,
        "One|Zulu|Victor|Romeo|Sierra|Whiskey|X-ray|Two|India|Papa|alpha");

    vec_str_insert(&v1, 4, strdup("beta"));
    const char* s2 = vec_str_get(&v1, 4);
    check_str_eq(tinfo, s2, "beta");
    vec_str_match(tinfo, &v1,
                  "One|Zulu|Victor|Romeo|beta|Sierra|Whiskey|X-ray|Two|"
                  "India|Papa|alpha");

    const char* s0 = vec_str_get(&v1, 0);
    check_str_eq(tinfo, s0, "One");
    const char* s5 = vec_str_get(&v1, 5);
    check_str_eq(tinfo, s5, "Sierra");
    const char* s6 = vec_str_get(&v1, 6);
    check_str_eq(tinfo, s6, "Whiskey");

    vec_str_set(&v1, 6, strdup("gamma"));
    const char* s6a = vec_str_get(&v1, 6);
    check_str_eq(tinfo, s6a, "gamma");
    vec_str_match(tinfo, &v1,
                  "One|Zulu|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_str_set(&v1, 0, strdup("A0"));
    const char* s0a = vec_str_get(&v1, 0);
    check_str_eq(tinfo, s0a, "A0");
    vec_str_match(tinfo, &v1,
                  "A0|Zulu|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_str_insert(&v1, 2, strdup("B2"));
    const char* s2a = vec_str_get(&v1, 2);
    check_str_eq(tinfo, s2a, "B2");
    vec_str_match(tinfo, &v1,
                  "A0|Zulu|B2|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_str_remove(&v1, 4);
    vec_str_match(tinfo, &v1,
                  "A0|Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_str_remove(&v1, 0);
    vec_str_match(tinfo, &v1,
                  "Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_str_remove(&v1, vec_str_size(&v1) - 1);
    vec_str_match(tinfo, &v1,
                  "Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa");

    vec_str_replace(&v1, 3, strdup("Hairy"));
    vec_str_match(tinfo, &v1,
                  "Zulu|B2|Victor|Hairy|Sierra|gamma|X-ray|Two|"
                  "India|Papa");
    char* x = vec_str_take(&v1, 1);
    check_str_eq(tinfo, x, "B2");
    free(x);
    vec_str_match(tinfo, &v1,
                  "Zulu|Victor|Hairy|Sierra|gamma|X-ray|Two|India|Papa");

    int index;
    index = vec_str_find(&v1, "Two");
    vec_check_found(tinfo, index, 6);
    index = vec_str_find(&v1, "two");
    vec_check_found(tinfo, index, VEC_NOT_FOUND);
    index = vec_str_find(&v1, "Papa");
    vec_check_found(tinfo, index, 8);
    index = vec_str_find(&v1, "Zulu");
    vec_check_found(tinfo, index, 0);
    index = vec_str_find_last(&v1, "two");
    vec_check_found(tinfo, index, VEC_NOT_FOUND);
    index = vec_str_find_last(&v1, "Zulu");
    vec_check_found(tinfo, index, 0);
    index = vec_str_find_last(&v1, "Two");
    vec_check_found(tinfo, index, 6);

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

    VecStr v4 = vec_str_alloc_split("one\ttwo\tthree\tfour\tfive", "\t");
    vec_str_match(tinfo, &v4, "one|two|three|four|five");
    vec_str_free(&v4);
    VecStr v5 =
        vec_str_alloc_split("oneSEPtwoSEPthreeSEPfourSEPfiveSEPsix", "SEP");
    vec_str_match(tinfo, &v5, "one|two|three|four|five|six");
    vec_str_free(&v5);
}

void vec_str_merge_tests(tinfo* tinfo) {
    VecStr v1 = vec_str_alloc_cap(7);
    vec_str_check_size_cap(tinfo, &v1, 0, 7);
    check_join(tinfo, &v1, ":", "");
    check_join(tinfo, &v1, NULL, "");
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
    vec_str_check_size_cap(tinfo, &v1, 5, 7);
    vec_str_match(tinfo, &v1, "one|two|three|four|five");

    VecStr v2 =
        vec_str_alloc_split("six\tseven\teight\tnine\tten\televen", "\t");
    vec_str_match(tinfo, &v2, "six|seven|eight|nine|ten|eleven");
    vec_str_check_size_cap(tinfo, &v2, 6, 32);

    vec_str_merge(&v1, &v2);
    vec_str_match(
        tinfo, &v1,
        "one|two|three|four|five|six|seven|eight|nine|ten|eleven");
    vec_str_check_size_cap(tinfo, &v1, 11, 11);
    vec_str_check_size_cap(tinfo, &v2, 0, 0);

    // v2 already freed
    vec_str_free(&v1);
}

void vec_str_sort_tests(tinfo* tinfo) {
    VecStr v1 = vec_str_alloc_split(
        "Zulu|Victor|Hairy|Sierra|gamma|X-ray|Two|India|Papa", "|");
    vec_str_match(tinfo, &v1,
                  "Zulu|Victor|Hairy|Sierra|gamma|X-ray|Two|India|Papa");

    int index;
    index = vec_str_find(&v1, "Zulu");
    vec_check_found(tinfo, index, 0);
    index = vec_str_find(&v1, "Papa");
    vec_check_found(tinfo, index, 8);
    index = vec_str_find(&v1, "Sierra");
    vec_check_found(tinfo, index, 3);
    index = vec_str_find(&v1, "Sierrb");
    vec_check_found(tinfo, index, VEC_NOT_FOUND);

    vec_str_sort(&v1);
    vec_str_match(tinfo, &v1,
                  "Hairy|India|Papa|Sierra|Two|Victor|X-ray|Zulu|gamma");
    vec_str_push(&v1, strdup("Alpha"));
    vec_str_push(&v1, strdup("kilo"));
    vec_str_match(
        tinfo, &v1,
        "Hairy|India|Papa|Sierra|Two|Victor|X-ray|Zulu|gamma|Alpha|kilo");
    vec_str_sort(&v1);
    vec_str_match(
        tinfo, &v1,
        "Alpha|Hairy|India|Papa|Sierra|Two|Victor|X-ray|Zulu|gamma|kilo");

    index = vec_str_find(&v1, "Zulu");
    vec_check_found(tinfo, index, 8);
    index = vec_str_search(&v1, "Zulu");
    vec_check_found(tinfo, index, 8);

    index = vec_str_find(&v1, "Papa");
    vec_check_found(tinfo, index, 3);
    index = vec_str_search(&v1, "Papa");
    vec_check_found(tinfo, index, 3);

    index = vec_str_find(&v1, "Sierra");
    vec_check_found(tinfo, index, 4);
    index = vec_str_search(&v1, "Sierra");
    vec_check_found(tinfo, index, 4);

    index = vec_str_find(&v1, "Sierrb");
    vec_check_found(tinfo, index, VEC_NOT_FOUND);
    index = vec_str_search(&v1, "Sierrb");
    vec_check_found(tinfo, index, VEC_NOT_FOUND);

    index = vec_str_find(&v1, "Alpha");
    vec_check_found(tinfo, index, 0);
    index = vec_str_search(&v1, "Alpha");
    vec_check_found(tinfo, index, 0);

    index = vec_str_find(&v1, "kilo");
    vec_check_found(tinfo, index, 10);
    index = vec_str_search(&v1, "kilo");
    vec_check_found(tinfo, index, 10);

    vec_str_match(
        tinfo, &v1,
        "Alpha|Hairy|India|Papa|Sierra|Two|Victor|X-ray|Zulu|gamma|kilo");
    vec_str_add(&v1, strdup("Aardvark"));
    vec_str_match(tinfo, &v1,
                  "Aardvark|Alpha|Hairy|India|Papa|Sierra|Two|Victor|X-ray|"
                  "Zulu|gamma|kilo");
    vec_str_add(&v1, strdup("limbo"));
    vec_str_match(tinfo, &v1,
                  "Aardvark|Alpha|Hairy|India|Papa|Sierra|Two|Victor|X-ray|"
                  "Zulu|gamma|kilo|limbo");
    vec_str_add(&v1, strdup("Oscar"));
    vec_str_match(tinfo, &v1,
                  "Aardvark|Alpha|Hairy|India|Oscar|Papa|Sierra|Two|Victor|"
                  "X-ray|Zulu|gamma|kilo|limbo");
    vec_str_add(&v1, strdup("Oscar"));
    vec_str_match(tinfo, &v1,
                  "Aardvark|Alpha|Hairy|India|Oscar|Oscar|Papa|Sierra|Two|"
                  "Victor|X-ray|Zulu|gamma|kilo|limbo");
    vec_str_free(&v1);
}

void vec_str_match(tinfo* tinfo, const VecStr* v, const char* expected) {
    char* out = vec_str_join(v, "|");
    check_str_eq(tinfo, out, expected);
    free(out);
}

void vec_str_check_size_cap(tinfo* tinfo, const VecStr* v, int size,
                            int cap) {
    tinfo->total++;
    if (vec_str_size(v) != size) {
        fprintf(stderr, "FAIL: %s vec_str_size() expected %d, got %d\n",
                tinfo->tag, size, vec_str_size(v));
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_str_isempty(v) != (size == 0)) {
        fprintf(stderr,
                "FAIL: %s vec_stry_isempty() expected %s, got %s size=%d\n",
                tinfo->tag, bool_to_str(size == 0),
                bool_to_str(vec_str_isempty(v)), size);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_str_cap(v) != cap) {
        fprintf(stderr, "FAIL: %s vec_str_cap() expected %d, got %d\n",
                tinfo->tag, cap, vec_str_cap(v));
    } else
        tinfo->ok++;
}

void vec_str_same(tinfo* tinfo, const VecStr* v1, const VecStr* v2) {
    tinfo->total++;
    if (!vec_str_equal(v1, v2)) {
        fprintf(stderr,
                "FAIL: %s vec_str_equal() expected true, got false\n",
                tinfo->tag);
    } else
        tinfo->ok++;
}

void check_join(tinfo* tinfo, const VecStr* vec, const char* sep,
                const char* expected) {
    tinfo->total++;
    char* actual = vec_str_join(vec, sep);
    if (strcmp(actual, expected))
        fprintf(stderr,
                "FAIL: %s vec_str_join() expected\n\"%s\", got\n\"%s\"\n",
                tinfo->tag, expected, actual);
    else
        tinfo->ok++;
    free(actual);
}
