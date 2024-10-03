// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str_test.h"
#include "exit.h"
#include "str.h"
#include "vec_str.h"
#include "vecs_test.h"
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
static void test_split_chr(tinfo*);
static void test_split_ws(tinfo*);
static void test_read_lines(tinfo*);

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
    tinfo->tag = "test_split_chr";
    test_split_chr(tinfo);
    tinfo->tag = "test_split_ws";
    test_split_ws(tinfo);
    tinfo->tag = "test_read_lines";
    test_read_lines(tinfo);

    tinfo->tag = "vec_str_tests continued";
    if (tinfo->verbose)
        puts(tinfo->tag);
    VecStr v1 = vec_str_alloc(); // default of 0 size and OWNS
    check_size_cap(tinfo, &v1, 0, 0);

    VecStr v2 = vec_str_copy(&v1, BORROWS);
    check_size_cap(tinfo, &v1, 0, 0);

    tinfo->total++;
    int WORD_COUNT = sizeof(WORDS) / sizeof(char*);
    for (int i = 0; i < WORD_COUNT; ++i) {
        int size = i == 0                   ? 0
                   : i <= VEC_INITIAL_CAP   ? VEC_INITIAL_CAP
                   : i <= VEC_INITIAL_CAPx2 ? VEC_INITIAL_CAPx2
                                            : VEC_INITIAL_CAPx4;
        check_size_cap(tinfo, &v1, i, size);
        vec_str_push(&v1, strdup(WORDS[i]));
        if (i < 10) {
            check_size_cap(tinfo, &v2, i, i == 0 ? 0 : VEC_INITIAL_CAP);
            vec_str_push(&v2, (char*)WORDS[i]);
        }
    }
    tinfo->ok++;

    check_size_cap(tinfo, &v1, WORD_COUNT, VEC_INITIAL_CAPx4);
    check_size_cap(tinfo, &v2, 10, VEC_INITIAL_CAP);
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
    check_size_cap(tinfo, &v1, 10, VEC_INITIAL_CAPx4);
    check_size_cap(tinfo, &v2, 10, VEC_INITIAL_CAP);
    equal(tinfo, &v1, &v2);

    vec_str_push(&v1, strdup("alpha"));
    const char* s1 = vec_str_get_last(&v1);
    check_str_eq(tinfo, s1, "alpha");
    s1 = VEC_GET_LAST(&v1);
    check_str_eq(tinfo, s1, "alpha");
    match(
        tinfo, &v1,
        "One|Zulu|Victor|Romeo|Sierra|Whiskey|X-ray|Two|India|Papa|alpha");

    vec_str_insert(&v1, 4, "beta"); // valgrind says strdup here would leak!
    const char* s2 = vec_str_get(&v1, 4); // but see "B2" later
    check_str_eq(tinfo, s2, "beta");
    s2 = VEC_GET(&v1, 4);
    check_str_eq(tinfo, s2, "beta");
    match(tinfo, &v1,
          "One|Zulu|Victor|Romeo|beta|Sierra|Whiskey|X-ray|Two|"
          "India|Papa|alpha");

    const char* s0 = vec_str_get(&v1, 0);
    check_str_eq(tinfo, s0, "One");
    s0 = VEC_GET(&v1, 0);
    check_str_eq(tinfo, s0, "One");
    s0 = vec_str_get_first(&v1);
    check_str_eq(tinfo, s0, "One");
    s0 = VEC_GET_FIRST(&v1);
    check_str_eq(tinfo, s0, "One");
    const char* s5 = vec_str_get(&v1, 5);
    check_str_eq(tinfo, s5, "Sierra");
    s5 = VEC_GET(&v1, 5);
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

    vec_str_insert(&v1, 2, strdup("B2")); // this is fine (& expected)
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

    vec_str_remove(&v1, VEC_SIZE(&v1) - 1);
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
    check_size_cap(tinfo, &v1, 0, VEC_INITIAL_CAPx4);
    vec_str_push(&v1, strdup("more"));
    const char* more = vec_str_get(&v1, 0);
    check_str_eq(tinfo, more, "more");
    match(tinfo, &v1, "more");
    check_size_cap(tinfo, &v1, 1, VEC_INITIAL_CAPx4);
    vec_str_free(&v1);
    check_size_cap(tinfo, &v1, 0, 0);
    vec_str_clear(&v2);
    check_size_cap(tinfo, &v2, 0, VEC_INITIAL_CAP);
    vec_str_free(&v2);
    check_size_cap(tinfo, &v2, 0, 0);

    VecStr v4 = split_str("one\ttwo\tthree\tfour\tfive", "\t");
    match(tinfo, &v4, "one|two|three|four|five");
    vec_str_free(&v4);
    VecStr v5 = split_str("oneSEPtwoSEPthreeSEPfourSEPfiveSEPsix", "SEP");
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

    VecStr v2 = split_str("six\tseven\teight\tnine\tten\televen", "\t");
    match(tinfo, &v2, "six|seven|eight|nine|ten|eleven");
    check_size_cap(tinfo, &v2, 6, VEC_INITIAL_CAP);

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
    VecStr v1 = split_str(
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

    VecStr v2 = split_str("Delta|bravo|Charlie|Bravo|delta|Alpha", "|");
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

static void test_split_chr(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    VecStr parts1 = split_chr("one\ttwo\tthree\tfour", '\t');
    check_int_eq(tinfo, VEC_SIZE(&parts1), 4);
    check_str_eq(tinfo, vec_str_get(&parts1, 0), "one");
    check_str_eq(tinfo, vec_str_get(&parts1, 1), "two");
    check_str_eq(tinfo, vec_str_get(&parts1, 2), "three");
    check_str_eq(tinfo, vec_str_get(&parts1, 3), "four");
    vec_str_free(&parts1);

    parts1 = split_chr("elephant", '\t');
    check_int_eq(tinfo, VEC_SIZE(&parts1), 1);
    check_str_eq(tinfo, vec_str_get(&parts1, 0), "elephant");
    vec_str_free(&parts1);

    parts1 = split_chr("moveto 3.5 7.45", ' ');
    check_int_eq(tinfo, VEC_SIZE(&parts1), 3);
    check_str_eq(tinfo, vec_str_get(&parts1, 0), "moveto");
    check_str_eq(tinfo, vec_str_get(&parts1, 1), "3.5");
    check_str_eq(tinfo, vec_str_get(&parts1, 2), "7.45");
    vec_str_free(&parts1);

    parts1 = split_chr("    ", '\t');
    check_int_eq(tinfo, VEC_SIZE(&parts1), 1); // no tabs so whole str
    vec_str_free(&parts1);

    parts1 = split_chr("    ", ' ');
    check_int_eq(tinfo, VEC_SIZE(&parts1), 0);
    vec_str_free(&parts1);

    parts1 = split_chr("", '\t');
    check_int_eq(tinfo, VEC_SIZE(&parts1), 0);
    vec_str_free(&parts1);

    parts1 = split_chr("", ' ');
    check_int_eq(tinfo, VEC_SIZE(&parts1), 0);
    vec_str_free(&parts1);

    VecStr parts2 = split_chr("width 240", ' ');
    check_int_eq(tinfo, VEC_SIZE(&parts2), 2);
    check_str_eq(tinfo, vec_str_get(&parts2, 0), "width");
    check_str_eq(tinfo, vec_str_get(&parts2, 1), "240");
    vec_str_free(&parts2);

    VecStr parts3 = split_chr("height 160", ' ');
    check_int_eq(tinfo, VEC_SIZE(&parts3), 2);
    check_str_eq(tinfo, vec_str_get(&parts3, 0), "height");
    check_str_eq(tinfo, vec_str_get(&parts3, 1), "160");
    vec_str_free(&parts3);

    VecStr parts4 =
        split_chr("rect x=10 y=10 w=220 h=140 fg=blue bg=green", ' ');
    check_int_eq(tinfo, VEC_SIZE(&parts4), 7);
    check_str_eq(tinfo, vec_str_get(&parts4, 0), "rect");
    check_str_eq(tinfo, vec_str_get(&parts4, 1), "x=10");
    check_str_eq(tinfo, vec_str_get(&parts4, 2), "y=10");
    check_str_eq(tinfo, vec_str_get(&parts4, 3), "w=220");
    check_str_eq(tinfo, vec_str_get(&parts4, 4), "h=140");
    check_str_eq(tinfo, vec_str_get(&parts4, 5), "fg=blue");
    check_str_eq(tinfo, vec_str_get(&parts4, 6), "bg=green");
    vec_str_free(&parts4);

    parts4 =
        split_chr("a b c d e f g h i j k l m n o p q r s t u v w x y z "
                  "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z",
                  ' ');
    check_int_eq(tinfo, VEC_SIZE(&parts4), 52);
    char buf[2];
    buf[0] = 'a';
    buf[1] = 0;
    for (int i = 0; i < 52; ++i) {
        check_str_eq(tinfo, vec_str_get(&parts4, i), buf);
        buf[0]++;
        if (i == 25)
            buf[0] = 'A';
    }
    vec_str_free(&parts4);
}

static void test_split_ws(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);

    VecStr parts1 = split_ws("  one\ttwo \tthree\tfour\n");
    check_int_eq(tinfo, VEC_SIZE(&parts1), 4);
    check_str_eq(tinfo, vec_str_get(&parts1, 0), "one");
    check_str_eq(tinfo, vec_str_get(&parts1, 1), "two");
    check_str_eq(tinfo, vec_str_get(&parts1, 2), "three");
    check_str_eq(tinfo, vec_str_get(&parts1, 3), "four");
    vec_str_free(&parts1);

    parts1 = split_ws("   moveto 3.5\t 7.45\n");
    check_int_eq(tinfo, VEC_SIZE(&parts1), 3);
    check_str_eq(tinfo, vec_str_get(&parts1, 0), "moveto");
    check_str_eq(tinfo, vec_str_get(&parts1, 1), "3.5");
    check_str_eq(tinfo, vec_str_get(&parts1, 2), "7.45");
    vec_str_free(&parts1);

    parts1 = split_ws("elephant");
    check_int_eq(tinfo, VEC_SIZE(&parts1), 1);
    check_str_eq(tinfo, vec_str_get(&parts1, 0), "elephant");
    vec_str_free(&parts1);

    parts1 = split_ws("   ");
    check_int_eq(tinfo, VEC_SIZE(&parts1), 0);
    vec_str_free(&parts1);

    parts1 = split_ws("");
    check_int_eq(tinfo, VEC_SIZE(&parts1), 0);
    vec_str_free(&parts1);

    VecStr parts2 = split_ws("width 240");
    check_int_eq(tinfo, VEC_SIZE(&parts2), 2);
    check_str_eq(tinfo, vec_str_get(&parts2, 0), "width");
    check_str_eq(tinfo, vec_str_get(&parts2, 1), "240");
    vec_str_free(&parts2);

    VecStr parts3 = split_ws("height 160");
    check_int_eq(tinfo, VEC_SIZE(&parts3), 2);
    check_str_eq(tinfo, vec_str_get(&parts3, 0), "height");
    check_str_eq(tinfo, vec_str_get(&parts3, 1), "160");
    vec_str_free(&parts3);

    VecStr parts4 = split_ws("rect x=10 y=10 w=220 h=140 fg=blue bg=green");
    check_int_eq(tinfo, VEC_SIZE(&parts4), 7);
    check_str_eq(tinfo, vec_str_get(&parts4, 0), "rect");
    check_str_eq(tinfo, vec_str_get(&parts4, 1), "x=10");
    check_str_eq(tinfo, vec_str_get(&parts4, 2), "y=10");
    check_str_eq(tinfo, vec_str_get(&parts4, 3), "w=220");
    check_str_eq(tinfo, vec_str_get(&parts4, 4), "h=140");
    check_str_eq(tinfo, vec_str_get(&parts4, 5), "fg=blue");
    check_str_eq(tinfo, vec_str_get(&parts4, 6), "bg=green");
    vec_str_free(&parts4);

    parts4 =
        split_ws("\ta b c d e f g h i j k l m n o p q r s t u v w x  y z "
                 "A B C D E F G H I J K L M N O P Q R S T U V W X  Y Z\n");
    check_int_eq(tinfo, VEC_SIZE(&parts4), 52);
    char buf[2];
    buf[0] = 'a';
    buf[1] = 0;
    for (int i = 0; i < 52; ++i) {
        check_str_eq(tinfo, vec_str_get(&parts4, i), buf);
        check_str_eq(tinfo, VEC_GET(&parts4, i), buf);
        buf[0]++;
        if (i == 25)
            buf[0] = 'A';
    }
    vec_str_free(&parts4);
}

static void test_read_lines(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    bool ok;
    VecStr vec = file_read_lines("st.sh", &ok);
    check_bool_eq(tinfo, true, ok);
    check_int_eq(tinfo, VEC_SIZE(&vec), 8);
    check_str_eq(tinfo, VEC_GET(&vec, 0), "rm -f core");
    check_str_eq(tinfo, VEC_GET(&vec, 1), "clc -sL rb");
    const char* s = VEC_GET(&vec, 2);
    check_bool_eq(tinfo, true, str_begins(s, "cppcheck"));
    check_str_eq(tinfo, VEC_GET(&vec, 5), "malloc_check.py *.[ch]");
    check_str_eq(tinfo, VEC_GET(&vec, 6), "cdoc.py -q");
    check_str_eq(tinfo, VEC_GET(&vec, 7), "git st");
    vec_str_free(&vec);
}

static void match(tinfo* tinfo, const VecStr* v, const char* expected) {
    char* out = vec_str_join(v, "|");
    check_str_eq(tinfo, out, expected);
    free(out);
}

static void check_size_cap(tinfo* tinfo, const VecStr* v, int size,
                           int cap) {
    tinfo->total++;
    if (VEC_SIZE(v) != size)
        WARN("FAIL: %s VEC_SIZE() expected %d != %d\n", tinfo->tag, size,
             VEC_SIZE(v));
    else
        tinfo->ok++;

    tinfo->total++;
    if (VEC_ISEMPTY(v) != (size == 0))
        WARN("FAIL: %s vec_stry_isempty() expected %s != %s size=%d\n",
             tinfo->tag, bool_to_str(size == 0),
             bool_to_str(VEC_ISEMPTY(v)), size);
    else
        tinfo->ok++;

    tinfo->total++;
    if (VEC_CAP(v) != cap)
        WARN("FAIL: %s VEC_CAP() expected %d != %d\n", tinfo->tag, cap,
             VEC_CAP(v));
    else
        tinfo->ok++;
}

static void equal(tinfo* tinfo, const VecStr* v1, const VecStr* v2) {
    tinfo->total++;
    if (!vec_str_equal(v1, v2))
        WARN("FAIL: %s vec_str_equal() expected true != false\n",
             tinfo->tag);
    else
        tinfo->ok++;
}

static void check_join(tinfo* tinfo, const VecStr* vec, const char* sep,
                       const char* expected) {
    tinfo->total++;
    char* actual = vec_str_join(vec, sep);
    if (!VEC_SIZE(vec) && !actual && !expected)
        tinfo->ok++;
    else if (actual && expected && strcmp(actual, expected))
        WARN("FAIL: %s vec_str_join() expected\n\"%s\" !=\n\"%s\"\n",
             tinfo->tag, expected, actual);
    else
        tinfo->ok++;
    free(actual);
}
