// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3

#include "set_int_test.h"
#include "exit.h"
#include "set_int.h"
#include "sets.h"
#include "vec_int.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

//#define REPORT_DEPTH

static void test_visit(tinfo* tinfo);
static void test_union(tinfo* tinfo);
static void test_intersection(tinfo* tinfo);
static void test_difference(tinfo* tinfo);
static void test_copy(tinfo* tinfo);
static void test_remove(tinfo* tinfo);
static void test_bigs(tinfo* tinfo);
static void test_simple(tinfo* tinfo);
static void test_contains(tinfo* tinfo);
static void check_all(tinfo* tinfo, const SetInt* set, int size);
static void check_order(tinfo* tinfo, const SetInt* set);
static void check_equal_ints(tinfo* tinfo, const SetInt* set,
                             const int* ints, int size);
// static void print_set(const SetInt* set, const char* name);

void set_int_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    tinfo->tag = "set_int test_simple";
    test_simple(tinfo);
    tinfo->tag = "set_int test_bigs";
    test_bigs(tinfo);
    tinfo->tag = "set_int test_contains";
    test_contains(tinfo);
    tinfo->tag = "set_int test_remove";
    test_remove(tinfo);
    tinfo->tag = "set_int test_copy";
    test_copy(tinfo);
    tinfo->tag = "set_int test_union";
    test_union(tinfo);
    tinfo->tag = "set_int test_difference";
    test_difference(tinfo);
    tinfo->tag = "set_int test_intersection";
    test_intersection(tinfo);
    tinfo->tag = "set_int test_visit";
    test_visit(tinfo);
}

typedef struct {
    int sum;
} SumState;

static void summer(int value, void* state_) {
    SumState* state = state_;
    state->sum += value;
}

static void test_visit(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SumState state = {0};
    SetInt set1 = set_int_alloc();
    for (int i = 1; i < 9; ++i)
        set_int_add(&set1, i);
    check_equal_ints(tinfo, &set1, (int[]){1, 2, 3, 4, 5, 6, 7, 8}, 8);
    set_int_visit(&set1, &state, summer);
    check_int_eq(tinfo, SET_SIZE(&set1), 8);
    check_int_eq(tinfo, state.sum, 36);
    for (int i = 1; i < 111; ++i)
        set_int_add(&set1, i);
    state.sum = 0;
    set_int_visit(&set1, &state, summer);
    check_int_eq(tinfo, SET_SIZE(&set1), 110);
    check_int_eq(tinfo, state.sum, 6105);
    set_int_free(&set1);
}

static void test_union(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetInt set1 = set_int_alloc();
    SetInt set2 = set_int_alloc();
    for (int i = 11; i < 23; ++i) {
        if (i % 2)
            set_int_add(&set1, i);
        else
            set_int_add(&set2, i);
    }
    set_int_add(&set1, 14);
    set_int_add(&set1, 22);
    check_equal_ints(tinfo, &set1, (int[]){11, 13, 14, 15, 17, 19, 21, 22},
                     8);
    check_equal_ints(tinfo, &set2, (int[]){12, 14, 16, 18, 20, 22}, 6);
    SetInt set3 = set_int_union(&set1, &set2);
    check_equal_ints(
        tinfo, &set3,
        (int[]){11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22}, 12);
    set_int_unite(&set1, &set2);
    check_equal_ints(
        tinfo, &set1,
        (int[]){11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22}, 12);
    tinfo->total++;
    if (!set_int_equal(&set1, &set3))
        WARN("FAIL: %s set1 != set3\n", tinfo->tag);
    else
        tinfo->ok++;
    set_int_clear(&set2);
    set2 = set_int_union(&set1, &set2);
    tinfo->total++;
    if (!set_int_equal(&set1, &set2))
        WARN("FAIL: %s set1 != set2\n", tinfo->tag);
    else
        tinfo->ok++;
    set_int_free(&set3);
    set_int_free(&set2);
    set_int_free(&set1);
}

static void test_intersection(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetInt set1 = set_int_alloc();
    SetInt set2 = set_int_alloc();
    for (int i = 11; i < 23; ++i) {
        if (i % 2)
            set_int_add(&set1, i);
        else
            set_int_add(&set2, i);
    }
    check_equal_ints(tinfo, &set1, (int[]){11, 13, 15, 17, 19, 21}, 6);
    check_equal_ints(tinfo, &set2, (int[]){12, 14, 16, 18, 20, 22}, 6);
    SetInt set3 = set_int_intersection(&set1, &set2);
    tinfo->total++;
    if (!SET_ISEMPTY(&set3))
        WARN("FAIL: %s set3 unexpectedly nonempty\n", tinfo->tag);
    else
        tinfo->ok++;
    for (int i = 12; i < 20; ++i)
        set_int_add(&set2, i);
    check_equal_ints(tinfo, &set2,
                     (int[]){12, 13, 14, 15, 16, 17, 18, 19, 20, 22}, 10);
    set_int_add(&set1, 14);
    set_int_clear(&set3);
    set3 = set_int_intersection(&set1, &set2);
    check_equal_ints(tinfo, &set3, (int[]){13, 14, 15, 17, 19}, 5);
    set_int_free(&set3);
    set_int_free(&set2);
    set_int_free(&set1);
}

static void test_difference(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetInt set1 = set_int_alloc();
    SetInt set2 = set_int_alloc();
    const int SIZE = 17;
    for (int i = 0; i < SIZE; ++i) {
        set_int_add(&set1, i);
        set_int_add(&set2, i);
    }
    check_equal_ints(
        tinfo, &set1,
        (int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
        17);
    tinfo->total++;
    if (!set_int_equal(&set1, &set2))
        WARN("FAIL: %s set1 != set2\n", tinfo->tag);
    else
        tinfo->ok++;
    for (int i = 2; i < SIZE - 3; i += 2)
        set_int_remove(&set2, i);
    check_equal_ints(tinfo, &set2,
                     (int[]){0, 1, 3, 5, 7, 9, 11, 13, 14, 15, 16}, 11);
    SetInt set3 = set_int_difference(&set1, &set2);
    check_equal_ints(tinfo, &set3,
                     (int[]){
                         2,
                         4,
                         6,
                         8,
                         10,
                         12,
                     },
                     6);
    set_int_clear(&set2);
    set_int_clear(&set1);
    for (int i = 0; i < 10; ++i) {
        set_int_add(&set1, i);
        set_int_add(&set2, i);
    }
    set_int_clear(&set3);
    set3 = set_int_difference(&set1, &set2);
    tinfo->total++;
    if (!SET_ISEMPTY(&set3))
        WARN("FAIL: %s set3 unexpectedly nonempty\n", tinfo->tag);
    else
        tinfo->ok++;
    set_int_clear(&set3);
    set_int_add(&set2, 10);
    set_int_add(&set2, 11);
    set_int_clear(&set3);
    set3 = set_int_difference(&set1, &set2);
    tinfo->total++;
    if (!SET_ISEMPTY(&set3))
        WARN("FAIL: %s set3 unexpectedly nonempty\n", tinfo->tag);
    else
        tinfo->ok++;
    set_int_clear(&set3);
    set3 = set_int_difference(&set2, &set1);
    check_equal_ints(tinfo, &set3, (int[]){10, 11}, 2);
    set_int_free(&set3);
    set_int_free(&set2);
    set_int_free(&set1);
}

static void test_copy(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetInt set = set_int_alloc();
    check_all(tinfo, &set, 0);
#ifdef REPORT_DEPTH
    const int SIZE = 1000;
#else
    const int SIZE = 51;
#endif
    int size = 0;
    for (int i = 0; i < SIZE - 11; ++i) {
        if (set_int_add(&set, rand() % SIZE))
            size++;
    }
    check_all(tinfo, &set, size);
    tinfo->total++;
    if (!set_int_equal(&set, &set))
        WARN("FAIL: %s set not equal to itself!\n", tinfo->tag);
    else
        tinfo->ok++;
    SetInt dup = set_int_copy(&set);
    tinfo->total++;
    if (!set_int_equal(&set, &dup))
        WARN("FAIL: %s set not equal to dup\n", tinfo->tag);
    else
        tinfo->ok++;
    set_int_free(&dup);
    set_int_free(&set);
}

static void test_simple(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetInt set = set_int_alloc();
    check_all(tinfo, &set, 0);
    for (int i = 30; i > 0; --i)
        set_int_add(&set, i);
    check_all(tinfo, &set, 30);
    check_equal_ints(tinfo, &set,
                     (int[]){1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                             11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                             21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
                     30);
    char* s = set_int_to_str(&set);
    check_str_eq(tinfo, s,
                 "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 "
                 "19 20 21 22 23 24 25 26 27 28 29 30");
    free(s);
    set_int_free(&set);
}

static void test_remove(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetInt set = set_int_alloc();
    check_all(tinfo, &set, 0);
    const int SIZE = 160000;
    int size = 0;
    for (int i = 0; i < SIZE; ++i) {
        if (set_int_add(&set, rand() % SIZE))
            size++;
    }
    int x = 0;
    for (int i = size / 3; i < size; ++i)
        if (!set_int_contains(&set, i)) {
            x = i;
            break;
        }
    int y = 0;
    for (int i = size / 5; i < size; ++i)
        if (!set_int_contains(&set, i)) {
            y = i;
            break;
        }
    // 3 bigger, 2 smaller, 2 in the middle
    const int to_remove[] = {SIZE + 1, SIZE + 121, SIZE + 1038, -91, -1,
                             x,        y};
    const int to_remove_size = (sizeof(to_remove) / sizeof(int));
    for (int i = 0; i < to_remove_size; ++i)
        set_int_add(&set, to_remove[i]);
    size += to_remove_size;
    for (int i = 0; i < SIZE / 10; ++i) {
        bool skip = false;
        const int z = rand();
        for (int j = 0; j < to_remove_size; ++j)
            if (z == to_remove[j]) {
                skip = true;
                break;
            }
        if (!skip && set_int_remove(&set, z))
            size--;
    }
    for (int i = 0; i < to_remove_size; ++i) {
        tinfo->total++;
        if (!set_int_remove(&set, to_remove[i]))
            WARN("FAIL: %s failed to remove %d\n", tinfo->tag,
                 to_remove[i]);
        else {
            size--;
            tinfo->ok++;
        }
    }
    check_all(tinfo, &set, size);
    set_int_free(&set);
}

static void test_bigs(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
#ifdef REPORT_DEPTH
    const int SIZE = 6;
#else
    const int SIZE = 5;
#endif
    int sizes[7] = {100};
    for (int i = 1; i < SIZE; ++i) {
        sizes[i] = sizes[i - 1] * 10;
    }
    SetInt set = set_int_alloc();
    check_all(tinfo, &set, 0);
    for (int i = 0; i < SIZE; ++i) {
        set_int_clear(&set);
        check_all(tinfo, &set, 0);
        int size = 0;
        for (int j = 0; j < sizes[i]; ++j) {
            if (set_int_add(&set, rand()))
                size++;
        }
        check_all(tinfo, &set, size);
    }
    for (int i = 0; i < SIZE; ++i) {
        set_int_clear(&set);
        check_all(tinfo, &set, 0);
        int size = 0;
        for (int j = 0; j < sizes[i]; ++j) {
            set_int_add(&set, j);
            size++;
        }
        check_all(tinfo, &set, size);
    }
    set_int_free(&set);
}

static void check_all(tinfo* tinfo, const SetInt* set, int size) {
    tinfo->total++;
    if (SET_SIZE(set) != size)
        WARN("FAIL: %s SET_SIZE() expected %d != %d\n", tinfo->tag, size,
             SET_SIZE(set));
    else
        tinfo->ok++;

#ifdef REPORT_DEPTH
    const char* tick = "\u2714";
#endif
    const char* cross = "\u2717";
    if (size) {
        tinfo->total++;
        int exp_rbtree_depth = (int)round(2 * log2f(size + 1));
        int depth = set_int_max_depth(set);
        if (depth > exp_rbtree_depth)
            WARN("FAIL: %s SetInt unexpectedly deep: size=%8d depth=%3d "
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

static void test_contains(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SetInt set = set_int_alloc();
    check_all(tinfo, &set, 0);
    int size = 0;
    for (int i = 0; i < 1000; ++i) {
        if (set_int_add(&set, rand() % 100))
            size++;
    }
    tinfo->total++;
    if (!set_int_add(&set, 111))
        WARN("FAIL: %s test_contains() failed to add 111\n", tinfo->tag);
    else
        tinfo->ok++;
    tinfo->total++;
    if (!set_int_add(&set, -2))
        WARN("FAIL: %s test_contains() failed to add 101\n", tinfo->tag);
    else
        tinfo->ok++;
    // These may already be present
    set_int_add(&set, 19);
    set_int_add(&set, 98);
    set_int_add(&set, 47);
    int values[] = {-2, 111, 19, 98, 47};
    for (int i = 0; i < (int)(sizeof(values) / sizeof(int)); ++i) {
        tinfo->total++;
        if (!set_int_contains(&set, values[i]))
            WARN("FAIL: %s test_contains() failed to find %d\n", tinfo->tag,
                 values[i]);
        else
            tinfo->ok++;
    }
    values[0] *= 10;
    values[1] *= -1;
    values[2] += 97;
    values[3] += 97;
    values[4] += 97;
    for (int i = 0; i < (int)(sizeof(values) / sizeof(int)); ++i) {
        tinfo->total++;
        if (set_int_contains(&set, values[i]))
            WARN("FAIL: %s test_contains() unexpectedly found %d\n",
                 tinfo->tag, values[i]);
        else
            tinfo->ok++;
    }
    set_int_free(&set);
}

static void check_order(tinfo* tinfo, const SetInt* set) {
    tinfo->total++;
    bool ok = true;
    VecInt vec = set_int_to_vec(set);
    for (int i = 1; i < VEC_SIZE(&vec); ++i)
        if (vec_int_get(&vec, i - 1) > vec_int_get(&vec, i)) {
            ok = false;
            break;
        }
    if (ok)
        tinfo->ok++;
    else
        WARN("FAIL: %s check_order\n", tinfo->tag);
    vec_int_free(&vec);
}

static void check_equal_ints(tinfo* tinfo, const SetInt* set,
                             const int* ints, int size) {
    tinfo->total++;
    const int SIZE = SET_SIZE(set);
    if (SIZE != size)
        WARN("FAIL: %s check_equal_ints set size %d expected %d\n",
             tinfo->tag, SIZE, size);
    else
        tinfo->ok++;
    tinfo->total++;
    bool ok = true;
    for (int i = 0; i < SIZE; ++i) {
        int x = ints[i];
        if (!set_int_contains(set, x)) {
            ok = false;
            break;
        }
    }
    if (ok)
        tinfo->ok++;
    else
        WARN("FAIL: %s check_equal_ints\n", tinfo->tag);
}

/*
static void print_set(const SetInt* set, const char* name) {
    char* s = set_int_to_str(set);
    printf("%s{%s}\n", name ? name : "", s);
    free(s);
}
*/
