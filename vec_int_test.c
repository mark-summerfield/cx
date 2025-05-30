// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3

#include "vec_int_test.h"
#include "exit.h"
#include "str.h"
#include "vec_int.h"
#include "vecs_test.h"
#include <stdlib.h>

static void check_size_cap(tinfo* tinfo, VecInt* v, int size, int cap);
static void match(tinfo* tinfo, VecInt* v, char* expected);
static void equal(tinfo* tinfo, VecInt* v1, VecInt* v2);
static void merge_tests(tinfo* tinfo);

void vec_int_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    merge_tests(tinfo);

    VecInt v1 = vec_int_alloc(); // default of 0
    check_size_cap(tinfo, &v1, 0, 0);

    VecInt v2 = vec_int_copy(&v1);
    check_size_cap(tinfo, &v1, 0, 0);

    tinfo->total++;
    for (int i = 1; i <= 35; ++i) {
        vec_int_push(&v1, i);
        if (i < 10)
            vec_int_push(&v2, i);
        int size = i <= VEC_INITIAL_CAP     ? VEC_INITIAL_CAP
                   : i <= VEC_INITIAL_CAPx2 ? VEC_INITIAL_CAPx2
                                            : VEC_INITIAL_CAPx4;
        check_size_cap(tinfo, &v1, i, size);
    }
    tinfo->ok++;

    check_size_cap(tinfo, &v1, 35, VEC_INITIAL_CAPx4);
    check_size_cap(tinfo, &v2, 9, VEC_INITIAL_CAP);
    match(tinfo, &v1,
          "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 "
          "20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35");
    match(tinfo, &v2, "1 2 3 4 5 6 7 8 9");

    for (int i = 35; i > 9; --i) {
        int v = vec_int_pop(&v1);
        check_int_eq(tinfo, v, i);
    }
    match(tinfo, &v1, "1 2 3 4 5 6 7 8 9");
    equal(tinfo, &v1, &v2);

    vec_int_push(&v1, -99);
    match(tinfo, &v1, "1 2 3 4 5 6 7 8 9 -99");

    vec_int_insert(&v1, 4, -555);
    match(tinfo, &v1, "1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_insert(&v1, 0, 21);
    match(tinfo, &v1, "21 1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_insert(&v1, 0, 17);
    match(tinfo, &v1, "17 21 1 2 3 4 -555 5 6 7 8 9 -99");

    VecInt v3 = vec_int_copy(&v1);
    check_size_cap(tinfo, &v3, VEC_SIZE(&v1), VEC_SIZE(&v1));

    int x = vec_int_get(&v1, 0);
    check_int_eq(tinfo, x, 17);
    x = VEC_GET(&v1, 0);
    check_int_eq(tinfo, x, 17);
    x = vec_int_get_first(&v1);
    check_int_eq(tinfo, x, 17);
    x = VEC_GET_FIRST(&v1);
    check_int_eq(tinfo, x, 17);
    x = vec_int_get(&v1, 4);
    check_int_eq(tinfo, x, 3);
    x = VEC_GET(&v1, 4);
    check_int_eq(tinfo, x, 3);
    x = vec_int_get_last(&v1);
    check_int_eq(tinfo, x, -99);
    x = VEC_GET_LAST(&v1);
    check_int_eq(tinfo, x, -99);

    vec_int_remove(&v1, 1);
    match(tinfo, &v1, "17 1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_remove(&v1, 0);
    match(tinfo, &v1, "1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_set(&v1, 2, -33);
    match(tinfo, &v1, "1 2 -33 4 -555 5 6 7 8 9 -99");
    vec_int_set(&v1, 10, 10);
    match(tinfo, &v1, "1 2 -33 4 -555 5 6 7 8 9 10");
    vec_int_set(&v1, 0, 0);
    match(tinfo, &v1, "0 2 -33 4 -555 5 6 7 8 9 10");

    x = vec_int_replace(&v1, 4, 111);
    check_int_eq(tinfo, x, -555);
    match(tinfo, &v1, "0 2 -33 4 111 5 6 7 8 9 10");

    vec_int_remove(&v1, 7);
    match(tinfo, &v1, "0 2 -33 4 111 5 6 8 9 10");
    vec_int_remove(&v1, 7);
    match(tinfo, &v1, "0 2 -33 4 111 5 6 9 10");

    x = vec_int_take(&v1, 0);
    check_int_eq(tinfo, x, 0);
    match(tinfo, &v1, "2 -33 4 111 5 6 9 10");
    x = vec_int_take(&v1, 7);
    check_int_eq(tinfo, x, 10);
    match(tinfo, &v1, "2 -33 4 111 5 6 9");

    int index;
    index = vec_int_find(&v1, 8);
    check_found(tinfo, index, VEC_NOT_FOUND);

    index = vec_int_find_last(&v1, 8);
    check_found(tinfo, index, VEC_NOT_FOUND);

    index = vec_int_find(&v1, 2);
    check_found(tinfo, index, 0);

    index = vec_int_find(&v1, 111);
    check_found(tinfo, index, 3);

    index = vec_int_find_last(&v1, 111);
    check_found(tinfo, index, 3);

    index = vec_int_find(&v1, 9);
    check_found(tinfo, index, 6);

    match(tinfo, &v3, "17 21 1 2 3 4 -555 5 6 7 8 9 -99");
    vec_int_sort(&v3);
    match(tinfo, &v3, "-555 -99 1 2 3 4 5 6 7 8 9 17 21");

    index = vec_int_search(&v3, 11);
    check_found(tinfo, index, VEC_NOT_FOUND);
    index = vec_int_find(&v3, 11);
    check_found(tinfo, index, VEC_NOT_FOUND);

    index = vec_int_search(&v3, 21);
    check_found(tinfo, index, 12);
    index = vec_int_find(&v3, 21);
    check_found(tinfo, index, 12);

    index = vec_int_search(&v3, -555);
    check_found(tinfo, index, 0);
    index = vec_int_find(&v3, -555);
    check_found(tinfo, index, 0);

    index = vec_int_search(&v3, 7);
    check_found(tinfo, index, 8);
    index = vec_int_find(&v3, 7);
    check_found(tinfo, index, 8);

    match(tinfo, &v3, "-555 -99 1 2 3 4 5 6 7 8 9 17 21");
    vec_int_add(&v3, -711);
    match(tinfo, &v3, "-711 -555 -99 1 2 3 4 5 6 7 8 9 17 21");
    vec_int_add(&v3, 45);
    match(tinfo, &v3, "-711 -555 -99 1 2 3 4 5 6 7 8 9 17 21 45");
    vec_int_add(&v3, 11);
    match(tinfo, &v3, "-711 -555 -99 1 2 3 4 5 6 7 8 9 11 17 21 45");
    vec_int_add(&v3, 11);
    match(tinfo, &v3, "-711 -555 -99 1 2 3 4 5 6 7 8 9 11 11 17 21 45");
    vec_int_add(&v3, 12);
    match(tinfo, &v3, "-711 -555 -99 1 2 3 4 5 6 7 8 9 11 11 12 17 21 45");
    vec_int_add(&v3, -822);
    match(tinfo, &v3,
          "-822 -711 -555 -99 1 2 3 4 5 6 7 8 9 11 11 12 17 21 45");
    vec_int_add(&v3, 57);
    match(tinfo, &v3,
          "-822 -711 -555 -99 1 2 3 4 5 6 7 8 9 11 11 12 17 21 45 57");
    vec_int_add(&v3, 10);
    match(tinfo, &v3,
          "-822 -711 -555 -99 1 2 3 4 5 6 7 8 9 10 11 11 12 17 21 45 57");
    vec_int_add(&v3, 10);
    match(
        tinfo, &v3,
        "-822 -711 -555 -99 1 2 3 4 5 6 7 8 9 10 10 11 11 12 17 21 45 57");

    index = vec_int_find(&v3, 11);
    check_found(tinfo, index, 15);
    index = vec_int_find_last(&v3, 11);
    check_found(tinfo, index, 16);

    vec_int_clear(&v1);
    check_size_cap(tinfo, &v1, 0, VEC_INITIAL_CAPx4);
    vec_int_push(&v1, -19);
    match(tinfo, &v1, "-19");
    vec_int_free(&v1);
    check_size_cap(tinfo, &v1, 0, 0);
    vec_int_clear(&v2);
    check_size_cap(tinfo, &v2, 0, VEC_INITIAL_CAP);
    vec_int_free(&v2);
    check_size_cap(tinfo, &v2, 0, 0);
    vec_int_free(&v3);
    check_size_cap(tinfo, &v3, 0, 0);
}

static void merge_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    VecInt v1 = vec_int_alloc_cap(7);
    check_size_cap(tinfo, &v1, 0, 7);
    for (int i = 1; i < 6; ++i)
        vec_int_push(&v1, i);
    check_size_cap(tinfo, &v1, 5, 7);
    match(tinfo, &v1, "1 2 3 4 5");

    VecInt v2 = vec_int_alloc();
    check_size_cap(tinfo, &v2, 0, 0);
    for (int i = 6; i < 12; ++i)
        vec_int_push(&v2, i);
    check_size_cap(tinfo, &v2, 6, VEC_INITIAL_CAP);
    match(tinfo, &v2, "6 7 8 9 10 11");

    vec_int_merge(&v1, &v2);
    match(tinfo, &v1, "1 2 3 4 5 6 7 8 9 10 11");
    check_size_cap(tinfo, &v1, 11, 11);
    check_size_cap(tinfo, &v2, 0, 0);
    // v2 already freed by merge
    vec_int_free(&v1);
}

static void match(tinfo* tinfo, VecInt* v, char* expected) {
    char* out = vec_int_to_str(v);
    check_str_eq(tinfo, out, expected);
    free(out);
}

static void check_size_cap(tinfo* tinfo, VecInt* v, int size, int cap) {
    tinfo->total++;
    if (VEC_SIZE(v) != size)
        WARN("FAIL: %s VEC_SIZE() expected %d != %d\n", tinfo->tag, size,
             VEC_SIZE(v));
    else
        tinfo->ok++;

    tinfo->total++;
    if (VEC_ISEMPTY(v) != (size == 0))
        WARN("FAIL: %s VEC_ISEMPTY() expected %s != %s size=%d\n",
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

static void equal(tinfo* tinfo, VecInt* v1, VecInt* v2) {
    tinfo->total++;
    if (!vec_int_equal(v1, v2))
        WARN("FAIL: %s vec_int_equal() expected true != false\n",
             tinfo->tag);
    else
        tinfo->ok++;
}
