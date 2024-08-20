// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "set_int_test.h"
#include "set_int.h"
#include "vec_int.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

//#define REPORT_DEPTH

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void test_bigs(tinfo* tinfo);
void test_simple(tinfo* tinfo);
void test_contains(tinfo* tinfo);
void check_all(tinfo* tinfo, const SetInt* set, int size);
void check_values(tinfo* tinfo, const SetInt* set, const char* expected);
void check_order(tinfo* tinfo, const SetInt* set);

void set_int_tests(tinfo* tinfo) {
    test_simple(tinfo);
    test_bigs(tinfo);
    test_contains(tinfo);
}

void test_simple(tinfo* tinfo) {
    SetInt set = set_int_alloc();
    check_all(tinfo, &set, 0);
    for (int i = 30; i > 0; --i)
        set_int_add(&set, i);
    check_all(tinfo, &set, 30);
    check_values(tinfo, &set,
                 "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 "
                 "23 24 25 26 27 28 29 30");
    set_int_free(&set);
}

void test_bigs(tinfo* tinfo) {
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
        if (sizes[i] > 10000) {
            break;
        }
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

void check_all(tinfo* tinfo, const SetInt* set, int size) {
    tinfo->total++;
    if (set_int_size(set) != size) {
        fprintf(stderr, "FAIL: %s set_int_size() expected %d, got %d\n",
                tinfo->tag, size, set_int_size(set));
    } else
        tinfo->ok++;

    if (size) {
        tinfo->total++;
        int exp_bintree_depth = (int)round(sqrt(size));
        int exp_rbtree_depth = (int)round(2 * log2f(size + 1));
        int depth = set_int_max_depth(set);
        if (depth > exp_rbtree_depth)
            fprintf(stderr,
                    "FAIL: %s SetInt unexpectedly deep: size=%8d depth=%3d "
                    "exp_rbtree_depth=%3d exp_bintree_depth=%5d\n",
                    tinfo->tag, size, depth, exp_rbtree_depth,
                    exp_bintree_depth);
        else
            tinfo->ok++;
        if (size < 1000001)
            check_order(tinfo, set);
#ifdef REPORT_DEPTH
        printf("size=%8d depth=%3d exp_rbtree_depth=%3d "
               "exp_bintree_depth=%5d\n",
               size, depth, exp_rbtree_depth, exp_bintree_depth);
#endif
    }
}

void test_contains(tinfo* tinfo) {
    SetInt set = set_int_alloc();
    check_all(tinfo, &set, 0);
    int size = 0;
    for (int i = 0; i < 1000; ++i) {
        if (set_int_add(&set, rand() % 100))
            size++;
    }
    tinfo->total++;
    if (!set_int_add(&set, 111))
        fprintf(stderr, "FAIL: %s test_contains() failed to add 111\n",
                tinfo->tag);
    else
        tinfo->ok++;
    tinfo->total++;
    if (!set_int_add(&set, -2))
        fprintf(stderr, "FAIL: %s test_contains() failed to add 101\n",
                tinfo->tag);
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
            fprintf(stderr, "FAIL: %s test_contains() failed to find %d\n",
                    tinfo->tag, values[i]);
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
            fprintf(stderr,
                    "FAIL: %s test_contains() unexpectedly found %d\n",
                    tinfo->tag, values[i]);
        else
            tinfo->ok++;
    }
    set_int_free(&set);
}

void check_values(tinfo* tinfo, const SetInt* set, const char* expected) {
    VecInt vec = set_int_to_vec(set);
    char* s = vec_int_tostring(&vec);
    tinfo->total++;
    if (strcmp(s, expected)) {
        fprintf(stderr,
                "FAIL: %s set_int_add() expected\n\t[%s], got\n\t[%s]\n",
                tinfo->tag, expected, s);
    } else
        tinfo->ok++;
    free(s);
    vec_int_free(&vec);
}

void check_order(tinfo* tinfo, const SetInt* set) {
    tinfo->total++;
    bool ok = true;
    VecInt vec = set_int_to_vec(set);
    for (int i = 1; i < vec_int_size(&vec); ++i) {
        if (vec_int_get(&vec, i - 1) > vec_int_get(&vec, i)) {
            ok = false;
            break;
        }
    }
    if (ok)
        tinfo->ok++;
    else
        fprintf(stderr, "FAIL: %s check_order\n", tinfo->tag);
    vec_int_free(&vec);
}
