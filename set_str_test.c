// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "set_str_test.h"
#include "set_str.h"
#include "sx.h"
#include "vec_str.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define REPORT_DEPTH

static void test_simple(tinfo* tinfo);
static void test_contains(tinfo* tinfo);
static void test_remove(tinfo* tinfo);
static void test_difference(tinfo* tinfo);
static void test_intersection(tinfo* tinfo);
static void test_union(tinfo* tinfo);
static void test_visit(tinfo* tinfo);
static void check_all(tinfo* tinfo, const SetStr* set, int size);
static void check_order(tinfo* tinfo, const SetStr* set);
static void check_set_strs(tinfo* tinfo, const SetStr* set, const char* s);
static void test_copy(tinfo* tinfo);
static SetStr prep_set(tinfo* tinfo);
static SetStr prep_set1(tinfo* tinfo);
// static void print_set(const SetStr* set, const char* name);

void set_str_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    tinfo->tag = "set_str test_simple";
    test_simple(tinfo);
    tinfo->tag = "set_str test_contains";
    test_contains(tinfo);
    tinfo->tag = "set_str test_remove";
    test_remove(tinfo);
    tinfo->tag = "set_str test_copy";
    test_copy(tinfo);
    tinfo->tag = "set_str test_union";
    test_union(tinfo);
    tinfo->tag = "set_str test_difference";
    test_difference(tinfo);
    tinfo->tag = "set_str test_intersection";
    test_intersection(tinfo);
    tinfo->tag = "set_str test_visit";
    test_visit(tinfo);
}

static void test_simple(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetStr set = prep_set(tinfo);
    check_set_strs(tinfo, &set, "eight|five|four|one|seven|six|three|two");
    set_str_free(&set);
}

static void test_contains(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetStr set = prep_set(tinfo);
    check_bool_eq(tinfo, set_str_contains(&set, "one"), true);
    check_bool_eq(tinfo, set_str_contains(&set, "eight"), true);
    check_bool_eq(tinfo, set_str_contains(&set, "two"), true);
    check_bool_eq(tinfo, set_str_contains(&set, "FOUR"), false);
    check_bool_eq(tinfo, set_str_contains(&set, ""), false);
    set_str_free(&set);
}

static void test_remove(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetStr set = prep_set(tinfo);
    check_bool_eq(tinfo, set_str_remove(&set, "one"), true);
    check_bool_eq(tinfo, set_str_remove(&set, "eight"), true);
    check_bool_eq(tinfo, set_str_remove(&set, "two"), true);
    check_bool_eq(tinfo, set_str_remove(&set, "FOUR"), false);
    check_bool_eq(tinfo, set_str_remove(&set, ""), false);
    check_set_strs(tinfo, &set, "five|four|seven|six|three");
    set_str_free(&set);
}

static void test_copy(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetStr set = prep_set(tinfo);
    tinfo->total++;
    if (!set_str_equal(&set, &set))
        fprintf(stderr, "FAIL: %s set not equal to itself!\n", tinfo->tag);
    else
        tinfo->ok++;
    SetStr dup = set_str_copy(&set, BORROWS);
    tinfo->total++;
    if (!set_str_equal(&set, &dup)) {
        fprintf(stderr, "FAIL: %s set not equal to dup\n", tinfo->tag);
    } else
        tinfo->ok++;
    set_str_free(&dup);
    set_str_free(&set);
}

static void test_union(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetStr set1 = prep_set(tinfo);
    SetStr set2 = prep_set1(tinfo);
    set_str_add(&set1, "ten");
    set_str_add(&set1, "eleven");
    SetStr set3 = set_str_union(&set1, &set2, BORROWS);
    check_set_strs(
        tinfo, &set3,
        "eight|eleven|five|four|nine|one|seven|six|ten|three|two");
    set_str_unite(&set1, &set2);
    check_set_strs(
        tinfo, &set1,
        "eight|eleven|five|four|nine|one|seven|six|ten|three|two");
    set_str_clear(&set3);
    set3 = set_str_union(&set1, &set2, BORROWS);
    tinfo->total++;
    if (!set_str_equal(&set1, &set3)) {
        fprintf(stderr, "FAIL: %s set1 != set3\n", tinfo->tag);
    } else
        tinfo->ok++;
    set_str_free(&set3);
    set_str_free(&set2);
    set_str_free(&set1);
}

static void test_difference(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetStr set1 = prep_set(tinfo);
    SetStr set2 = prep_set1(tinfo);
    set_str_add(&set1, "ten");
    set_str_add(&set1, "eleven");
    check_set_strs(tinfo, &set1,
                   "eight|eleven|five|four|one|seven|six|ten|three|two");
    check_set_strs(tinfo, &set2, "five|nine|one|seven|three");
    SetStr set3 = set_str_difference(&set1, &set2, BORROWS);
    check_set_strs(tinfo, &set3, "eight|eleven|four|six|ten|two");
    SetStr set4 = set_str_difference(&set3, &set3, BORROWS);
    check_bool_eq(tinfo, set_str_isempty(&set4), true);
    set_str_clear(&set3);
    set3 = set_str_difference(&set2, &set1, BORROWS);
    check_set_strs(tinfo, &set3, "nine");
    set_str_free(&set4);
    set_str_free(&set3);
    set_str_free(&set2);
    set_str_free(&set1);
}

static void test_intersection(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetStr set1 = prep_set(tinfo);
    SetStr set2 = prep_set1(tinfo);
    check_set_strs(tinfo, &set1, "eight|five|four|one|seven|six|three|two");
    check_set_strs(tinfo, &set2, "five|nine|one|seven|three");
    SetStr set3 = set_str_intersection(&set1, &set2, BORROWS);
    check_set_strs(tinfo, &set3, "five|one|seven|three");
    SetStr set4 = set_str_intersection(&set3, &set3, BORROWS);
    check_set_strs(tinfo, &set4, "five|one|seven|three");
    set_str_clear(&set4);
    set_str_clear(&set3);
    set3 = set_str_intersection(&set1, &set4, BORROWS);
    check_bool_eq(tinfo, set_str_isempty(&set3), true);
    set_str_add(&set4, "ABC");
    set_str_add(&set4, "XY");
    set_str_clear(&set3);
    set3 = set_str_intersection(&set1, &set4, BORROWS);
    check_bool_eq(tinfo, set_str_isempty(&set3), true);
    set_str_free(&set4);
    set_str_free(&set3);
    set_str_free(&set2);
    set_str_free(&set1);
}

#define NUM_LETTERS 26

typedef struct {
    int start_letter[NUM_LETTERS];
} StartLetters;

static void start_letters(const char* value, void* state_) {
    StartLetters* state = state_;
    state->start_letter[value[0] - 'a']++;
}

static void test_visit(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    StartLetters state = {0};
    for (int i = 0; i < NUM_LETTERS; i++)
        check_int_eq(tinfo, state.start_letter[i], 0);
    SetStr set = prep_set(tinfo);
    check_set_strs(tinfo, &set, "eight|five|four|one|seven|six|three|two");
    set_str_visit(&set, &state, start_letters);
    for (int i = 0; i < NUM_LETTERS; i++) {
        char letter = 'a' + i;
        switch (letter) {
        case 'e':
        case 'o':
            check_int_eq(tinfo, state.start_letter[i], 1);
            break;
        case 'f':
        case 's':
        case 't':
            check_int_eq(tinfo, state.start_letter[i], 2);
            break;
        default:
            check_int_eq(tinfo, state.start_letter[i], 0);
        }
    }
    for (int i = 0; i < NUM_LETTERS; i++)
        state.start_letter[i] = 0;
    set_str_add(&set, "nine");
    set_str_add(&set, "ten");
    set_str_visit(&set, &state, start_letters);
    for (int i = 0; i < NUM_LETTERS; i++) {
        char letter = 'a' + i;
        switch (letter) {
        case 'e':
        case 'o':
        case 'n':
            check_int_eq(tinfo, state.start_letter[i], 1);
            break;
        case 'f':
        case 's':
            check_int_eq(tinfo, state.start_letter[i], 2);
            break;
        case 't':
            check_int_eq(tinfo, state.start_letter[i], 3);
            break;
        default:
            check_int_eq(tinfo, state.start_letter[i], 0);
        }
    }
    set_str_free(&set);
}

static void check_all(tinfo* tinfo, const SetStr* set, int size) {
    tinfo->total++;
    if (set_str_size(set) != size) {
        fprintf(stderr, "FAIL: %s set_str_size() expected %d != %d\n",
                tinfo->tag, size, set_str_size(set));
    } else
        tinfo->ok++;

#ifdef REPORT_DEPTH
    const char* tick = "\u2714";
#endif
    const char* cross = "\u2717";
    if (size) {
        tinfo->total++;
        int exp_rbtree_depth = (int)round(2 * log2f(size + 1));
        int depth = set_str_max_depth(set);
        if (depth > exp_rbtree_depth)
            fprintf(stderr,
                    "FAIL: %s SetStr unexpectedly deep: size=%8d depth=%3d "
                    "2×lg(n)=%3d %s\n",
                    tinfo->tag, size, depth, exp_rbtree_depth, cross);
        else
            tinfo->ok++;
        if (size < 1000001)
            check_order(tinfo, set);
#ifdef REPORT_DEPTH
        printf("%-20s size=%8d depth=%3d 2×lg(n)=%3d %s\n", tinfo->tag,
               size, depth, exp_rbtree_depth,
               (depth < exp_rbtree_depth) ? tick : cross);
#endif
    }
}

static void check_order(tinfo* tinfo, const SetStr* set) {
    tinfo->total++;
    bool ok = true;
    VecStr vec = set_str_to_vec(set, BORROWS);
    for (int i = 1; i < vec_str_size(&vec); ++i)
        if (strcmp(vec_str_get(&vec, i - 1), vec_str_get(&vec, i)) > 0) {
            ok = false;
            break;
        }
    if (ok)
        tinfo->ok++;
    else
        fprintf(stderr, "FAIL: %s check_order\n", tinfo->tag);
    vec_str_free(&vec);
}

static void check_set_strs(tinfo* tinfo, const SetStr* set, const char* s) {
    tinfo->total++;
    char* a = set_str_join(set, "|");
    if (strcmp(a, s) != 0)
        fprintf(stderr,
                "FAIL: %s check_set_strs got\n\"%s\", expected\n\"%s\"\n",
                tinfo->tag, a, s);
    else
        tinfo->ok++;
    free(a);
}

static SetStr prep_set(tinfo* tinfo) {
    SetStr set = set_str_alloc(BORROWS);
    check_all(tinfo, &set, 0);
    char* a[] = {"one", "two", "three", "four",  "five",
                 "six", "two", "one",   "seven", "eight"};
    int a_sz = sizeof(a) / sizeof(char*);
    for (int i = 0; i < a_sz; i++)
        set_str_add(&set, a[i]);
    check_all(tinfo, &set, a_sz - 2); // -2 due to dropped duplicates
    return set;
}

static SetStr prep_set1(tinfo* tinfo) {
    SetStr set = set_str_alloc(BORROWS);
    check_all(tinfo, &set, 0);
    char* a[] = {"one", "three", "five", "nine", "one", "seven"};
    int a_sz = sizeof(a) / sizeof(char*);
    for (int i = 0; i < a_sz; i++)
        set_str_add(&set, a[i]);
    check_all(tinfo, &set, a_sz - 1); // -1 due to dropped duplicates
    return set;
}

/*
static void print_set(const SetStr* set, const char* name) {
    char* s = set_str_join(set, "|");
    printf("%s{%s}\n", name ? name : "", s);
    free(s);
}
*/
