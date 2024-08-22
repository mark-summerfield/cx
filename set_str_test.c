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
static void test_union(tinfo* tinfo);
static void check_all(tinfo* tinfo, const SetStr* set, int size);
static void check_order(tinfo* tinfo, const SetStr* set);
static void check_set_strs(tinfo* tinfo, const SetStr* set, const char* s);
static void check_equal_str(tinfo* tinfo, const char* s, const char* t);
static void check_bool(tinfo* tinfo, bool actual, bool expected);
static void test_copy(tinfo* tinfo);
static SetStr prep_set(tinfo* tinfo);
static SetStr prep_set1(tinfo* tinfo);
static void print_set(const SetStr* set, const char* name);
/*
static void test_intersection(tinfo* tinfo);
static void test_difference(tinfo* tinfo);
*/

void set_str_tests(tinfo* tinfo) {
    tinfo->tag = "test_simple";
    test_simple(tinfo);
    tinfo->tag = "test_contains";
    test_contains(tinfo);
    tinfo->tag = "test_remove";
    test_remove(tinfo);
    tinfo->tag = "test_copy";
    test_copy(tinfo);
    tinfo->tag = "test_union";
    // TODO
    //    test_union(tinfo);
    /*
    tinfo->tag = "test_difference";
    test_difference(tinfo);
    tinfo->tag = "test_intersection";
    test_intersection(tinfo);
    */
}

static void test_simple(tinfo* tinfo) {
    SetStr set = prep_set(tinfo);
    check_set_strs(tinfo, &set, "eight|five|four|one|seven|six|three|two");
    set_str_free(&set);
}

static void test_contains(tinfo* tinfo) {
    SetStr set = prep_set(tinfo);
    check_bool(tinfo, set_str_contains(&set, "one"), true);
    check_bool(tinfo, set_str_contains(&set, "eight"), true);
    check_bool(tinfo, set_str_contains(&set, "two"), true);
    check_bool(tinfo, set_str_contains(&set, "FOUR"), false);
    check_bool(tinfo, set_str_contains(&set, ""), false);
    set_str_free(&set);
}

static void test_remove(tinfo* tinfo) {
    SetStr set = prep_set(tinfo);
    check_bool(tinfo, set_str_remove(&set, "one"), true);
    check_bool(tinfo, set_str_remove(&set, "eight"), true);
    check_bool(tinfo, set_str_remove(&set, "two"), true);
    check_bool(tinfo, set_str_remove(&set, "FOUR"), false);
    check_bool(tinfo, set_str_remove(&set, ""), false);
    check_set_strs(tinfo, &set, "five|four|seven|six|three");
    set_str_free(&set);
}

static void test_copy(tinfo* tinfo) {
    SetStr set = prep_set(tinfo);
    tinfo->total++;
    if (!set_str_equal(&set, &set))
        fprintf(stderr, "FAIL: %s set not equal to itself!\n", tinfo->tag);
    else
        tinfo->ok++;
    SetStr dup = set_str_copy(&set, false);
    tinfo->total++;
    if (!set_str_equal(&set, &dup)) {
        fprintf(stderr, "FAIL: %s set not equal to dup\n", tinfo->tag);
    } else
        tinfo->ok++;
    set_str_free(&dup);
    set_str_free(&set);
}

static void test_union(tinfo* tinfo) {
    SetStr set1 = prep_set(tinfo);
    SetStr set2 = prep_set1(tinfo);
    set_str_add(&set1, "ten");
    set_str_add(&set1, "eleven");
    SetStr set3 = set_str_union(&set1, &set2, false);
    check_set_strs(
        tinfo, &set3,
        "eight|eleven|five|four|nine|one|seven|six|ten|three|two");
    set_str_unite(&set1, &set2);
    check_set_strs(
        tinfo, &set1,
        "eight|eleven|five|four|nine|one|seven|six|ten|three|two");
    set_str_clear(&set3);
    set3 = set_str_union(&set1, &set2, false);
    tinfo->total++;
    if (!set_str_equal(&set1, &set3)) {
        fprintf(stderr, "FAIL: %s set1 != set3\n", tinfo->tag);
    } else
        tinfo->ok++;
    set_str_free(&set3);
    set_str_free(&set2);
    set_str_free(&set1);
}

/*
static void test_intersection(tinfo* tinfo) {
    SetStr set1 = set_str_alloc(true);
    SetStr set2 = set_str_alloc(true);
    for (int i = 11; i < 23; ++i) {
        if (i % 2)
            set_str_add(&set1, i);
        else
            set_str_add(&set2, i);
    }
    check_equal_str(tinfo, &set1, (int[]){11, 13, 15, 17, 19, 21}, 6);
    check_equal_str(tinfo, &set2, (int[]){12, 14, 16, 18, 20, 22}, 6);
    SetStr set3 = set_str_intersection(&set1, &set2);
    tinfo->total++;
    if (!set_str_isempty(&set3)) {
        fprintf(stderr, "FAIL: %s set3 unexpectedly nonempty\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    for (int i = 12; i < 20; ++i)
        set_str_add(&set2, i);
    check_equal_str(tinfo, &set2,
                     (int[]){12, 13, 14, 15, 16, 17, 18, 19, 20, 22}, 10);
    set_str_add(&set1, 14);
    set_str_clear(&set3);
    set3 = set_str_intersection(&set1, &set2);
    check_equal_str(tinfo, &set3, (int[]){13, 14, 15, 17, 19}, 5);
    set_str_free(&set3);
    set_str_free(&set2);
    set_str_free(&set1);
}

static void test_difference(tinfo* tinfo) {
    SetStr set1 = set_str_alloc(true);
    SetStr set2 = set_str_alloc(true);
    const int SIZE = 17;
    for (int i = 0; i < SIZE; ++i) {
        set_str_add(&set1, i);
        set_str_add(&set2, i);
    }
    check_equal_str(
        tinfo, &set1,
        (int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
        17);
    tinfo->total++;
    if (!set_str_equal(&set1, &set2)) {
        fprintf(stderr, "FAIL: %s set1 != set2\n", tinfo->tag);
    } else
        tinfo->ok++;
    for (int i = 2; i < SIZE - 3; i += 2)
        set_str_remove(&set2, i);
    check_equal_str(tinfo, &set2,
                     (int[]){0, 1, 3, 5, 7, 9, 11, 13, 14, 15, 16}, 11);
    SetStr set3 = set_str_difference(&set1, &set2);
    check_equal_str(tinfo, &set3,
                     (int[]){
                         2,
                         4,
                         6,
                         8,
                         10,
                         12,
                     },
                     6);
    set_str_clear(&set2);
    set_str_clear(&set1);
    for (int i = 0; i < 10; ++i) {
        set_str_add(&set1, i);
        set_str_add(&set2, i);
    }
    set_str_clear(&set3);
    set3 = set_str_difference(&set1, &set2);
    tinfo->total++;
    if (!set_str_isempty(&set3)) {
        fprintf(stderr, "FAIL: %s set3 unexpectedly nonempty\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    set_str_clear(&set3);
    set_str_add(&set2, 10);
    set_str_add(&set2, 11);
    set_str_clear(&set3);
    set3 = set_str_difference(&set1, &set2);
    tinfo->total++;
    if (!set_str_isempty(&set3)) {
        fprintf(stderr, "FAIL: %s set3 unexpectedly nonempty\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    set_str_clear(&set3);
    set3 = set_str_difference(&set2, &set1);
    check_equal_str(tinfo, &set3, (int[]){10, 11}, 2);
    set_str_free(&set3);
    set_str_free(&set2);
    set_str_free(&set1);
}

*/

static void check_all(tinfo* tinfo, const SetStr* set, int size) {
    tinfo->total++;
    if (set_str_size(set) != size) {
        fprintf(stderr, "FAIL: %s set_str_size() expected %d, got %d\n",
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
    VecStr vec = set_str_to_vec(set, false);
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
}

static void check_equal_str(tinfo* tinfo, const char* s, const char* t) {
    tinfo->total++;
    if (strcmp(s, t) != 0)
        fprintf(stderr,
                "FAIL: %s check_equal_str got\n\"%s\", expected\n\"%s\"\n",
                tinfo->tag, s, t);
    else
        tinfo->ok++;
}

static void check_bool(tinfo* tinfo, bool actual, bool expected) {
    tinfo->total++;
    if (actual != expected)
        fprintf(stderr, "FAIL: %s check_bool got %s, expected %s\n",
                tinfo->tag, bool_to_str(actual), bool_to_str(expected));
    else
        tinfo->ok++;
}

static SetStr prep_set(tinfo* tinfo) {
    SetStr set = set_str_alloc(true);
    check_all(tinfo, &set, 0);
    char* a[] = {"one", "two", "three", "four",  "five",
                 "six", "two", "one",   "seven", "eight"};
    int a_sz = sizeof(a) / sizeof(char*);
    for (int i = 0; i < a_sz; i++)
        set_str_add(&set, strdup(a[i]));
    check_all(tinfo, &set, a_sz - 2); // -2 due to dropped duplicates
    return set;
}

static SetStr prep_set1(tinfo* tinfo) {
    SetStr set = set_str_alloc(true);
    check_all(tinfo, &set, 0);
    char* a[] = {"one", "three", "five", "nine", "one", "seven"};
    int a_sz = sizeof(a) / sizeof(char*);
    for (int i = 0; i < a_sz; i++)
        set_str_add(&set, strdup(a[i]));
    check_all(tinfo, &set, a_sz - 1); // -1 due to dropped duplicates
    return set;
}

static void print_set(const SetStr* set, const char* name) {
    char* s = set_str_join(set, "|");
    printf("%s{%s}\n", name ? name : "", s);
    free(s);
}
