// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_int_test.h"
#include "sx.h"
#include "vec_common_test.h"
#include "vec_int.h"
#include <stdlib.h>

void vec_int_check_size_cap(tinfo* tinfo, VecInt* v, int size, int cap);
void vec_int_match(tinfo* tinfo, VecInt* v, char* expected);
void vec_int_same(tinfo* tinfo, VecInt* v1, VecInt* v2);
void vec_int_print(VecInt* v);
void vec_str_tests(tinfo*, bool);
void vec_int_merge_tests(tinfo* tinfo);

void vec_int_tests(tinfo* tinfo) {
    vec_int_merge_tests(tinfo);

    VecInt v1 = vec_int_alloc(); // default of 32
    vec_int_check_size_cap(tinfo, &v1, 0, 32);

    VecInt v2 = vec_int_copy(&v1);
    vec_int_check_size_cap(tinfo, &v1, 0, 32);

    tinfo->total++;
    for (int i = 1; i <= 35; ++i) {
        vec_int_push(&v1, i);
        if (i < 10)
            vec_int_push(&v2, i);
        vec_int_check_size_cap(tinfo, &v1, i, i <= 32 ? 32 : 64);
    }
    tinfo->ok++;

    vec_int_check_size_cap(tinfo, &v1, 35, 64);
    vec_int_check_size_cap(tinfo, &v2, 9, 32);
    vec_int_match(tinfo, &v1,
                  "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 "
                  "20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35");
    vec_int_match(tinfo, &v2, "1 2 3 4 5 6 7 8 9");

    for (int i = 35; i > 9; --i) {
        int v = vec_int_pop(&v1);
        check_int_eq(tinfo, v, i);
    }
    vec_int_match(tinfo, &v1, "1 2 3 4 5 6 7 8 9");
    vec_int_same(tinfo, &v1, &v2);

    vec_int_push(&v1, -99);
    vec_int_match(tinfo, &v1, "1 2 3 4 5 6 7 8 9 -99");

    vec_int_insert(&v1, 4, -555);
    vec_int_match(tinfo, &v1, "1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_insert(&v1, 0, 21);
    vec_int_match(tinfo, &v1, "21 1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_insert(&v1, 0, 17);
    vec_int_match(tinfo, &v1, "17 21 1 2 3 4 -555 5 6 7 8 9 -99");

    VecInt v3 = vec_int_copy(&v1);
    vec_int_check_size_cap(tinfo, &v3, vec_int_size(&v1),
                           vec_int_size(&v1));

    int x = vec_int_get(&v1, 0);
    check_int_eq(tinfo, x, 17);
    x = vec_int_get(&v1, 4);
    check_int_eq(tinfo, x, 3);
    x = vec_int_get_last(&v1);
    check_int_eq(tinfo, x, -99);

    vec_int_remove(&v1, 1);
    vec_int_match(tinfo, &v1, "17 1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_remove(&v1, 0);
    vec_int_match(tinfo, &v1, "1 2 3 4 -555 5 6 7 8 9 -99");

    vec_int_set(&v1, 2, -33);
    vec_int_match(tinfo, &v1, "1 2 -33 4 -555 5 6 7 8 9 -99");
    vec_int_set(&v1, 10, 10);
    vec_int_match(tinfo, &v1, "1 2 -33 4 -555 5 6 7 8 9 10");
    vec_int_set(&v1, 0, 0);
    vec_int_match(tinfo, &v1, "0 2 -33 4 -555 5 6 7 8 9 10");

    x = vec_int_replace(&v1, 4, 111);
    check_int_eq(tinfo, x, -555);
    vec_int_match(tinfo, &v1, "0 2 -33 4 111 5 6 7 8 9 10");

    vec_int_remove(&v1, 7);
    vec_int_match(tinfo, &v1, "0 2 -33 4 111 5 6 8 9 10");
    vec_int_remove(&v1, 7);
    vec_int_match(tinfo, &v1, "0 2 -33 4 111 5 6 9 10");

    x = vec_int_take(&v1, 0);
    check_int_eq(tinfo, x, 0);
    vec_int_match(tinfo, &v1, "2 -33 4 111 5 6 9 10");
    x = vec_int_take(&v1, 7);
    check_int_eq(tinfo, x, 10);
    vec_int_match(tinfo, &v1, "2 -33 4 111 5 6 9");

    int index;
    index = vec_int_find(&v1, 8);
    vec_check_found(tinfo, index, VEC_NOT_FOUND);

    index = vec_int_find_last(&v1, 8);
    vec_check_found(tinfo, index, VEC_NOT_FOUND);

    index = vec_int_find(&v1, 2);
    vec_check_found(tinfo, index, 0);

    index = vec_int_find(&v1, 111);
    vec_check_found(tinfo, index, 3);

    index = vec_int_find_last(&v1, 111);
    vec_check_found(tinfo, index, 3);

    index = vec_int_find(&v1, 9);
    vec_check_found(tinfo, index, 6);

    vec_int_match(tinfo, &v3, "17 21 1 2 3 4 -555 5 6 7 8 9 -99");
    vec_int_sort(&v3);
    vec_int_match(tinfo, &v3, "-555 -99 1 2 3 4 5 6 7 8 9 17 21");

    index = vec_int_search(&v3, 11);
    vec_check_found(tinfo, index, VEC_NOT_FOUND);
    index = vec_int_find(&v3, 11);
    vec_check_found(tinfo, index, VEC_NOT_FOUND);

    index = vec_int_search(&v3, 21);
    vec_check_found(tinfo, index, 12);
    index = vec_int_find(&v3, 21);
    vec_check_found(tinfo, index, 12);

    index = vec_int_search(&v3, -555);
    vec_check_found(tinfo, index, 0);
    index = vec_int_find(&v3, -555);
    vec_check_found(tinfo, index, 0);

    index = vec_int_search(&v3, 7);
    vec_check_found(tinfo, index, 8);
    index = vec_int_find(&v3, 7);
    vec_check_found(tinfo, index, 8);

    vec_int_match(tinfo, &v3, "-555 -99 1 2 3 4 5 6 7 8 9 17 21");
    vec_int_add(&v3, -711);
    vec_int_match(tinfo, &v3, "-711 -555 -99 1 2 3 4 5 6 7 8 9 17 21");
    vec_int_add(&v3, 45);
    vec_int_match(tinfo, &v3, "-711 -555 -99 1 2 3 4 5 6 7 8 9 17 21 45");
    vec_int_add(&v3, 11);
    vec_int_match(tinfo, &v3,
                  "-711 -555 -99 1 2 3 4 5 6 7 8 9 11 17 21 45");
    vec_int_add(&v3, 11);
    vec_int_match(tinfo, &v3,
                  "-711 -555 -99 1 2 3 4 5 6 7 8 9 11 11 17 21 45");
    vec_int_add(&v3, 12);
    vec_int_match(tinfo, &v3,
                  "-711 -555 -99 1 2 3 4 5 6 7 8 9 11 11 12 17 21 45");
    vec_int_add(&v3, -822);
    vec_int_match(tinfo, &v3,
                  "-822 -711 -555 -99 1 2 3 4 5 6 7 8 9 11 11 12 17 21 45");
    vec_int_add(&v3, 57);
    vec_int_match(
        tinfo, &v3,
        "-822 -711 -555 -99 1 2 3 4 5 6 7 8 9 11 11 12 17 21 45 57");
    vec_int_add(&v3, 10);
    vec_int_match(
        tinfo, &v3,
        "-822 -711 -555 -99 1 2 3 4 5 6 7 8 9 10 11 11 12 17 21 45 57");
    vec_int_add(&v3, 10);
    vec_int_match(
        tinfo, &v3,
        "-822 -711 -555 -99 1 2 3 4 5 6 7 8 9 10 10 11 11 12 17 21 45 57");

    index = vec_int_find(&v3, 11);
    vec_check_found(tinfo, index, 15);
    index = vec_int_find_last(&v3, 11);
    vec_check_found(tinfo, index, 16);

    vec_int_clear(&v1);
    vec_int_check_size_cap(tinfo, &v1, 0, 64);
    vec_int_push(&v1, -19);
    vec_int_match(tinfo, &v1, "-19");
    vec_int_free(&v1);
    vec_int_check_size_cap(tinfo, &v1, 0, 0);
    vec_int_clear(&v2);
    vec_int_check_size_cap(tinfo, &v2, 0, 32);
    vec_int_free(&v2);
    vec_int_check_size_cap(tinfo, &v2, 0, 0);
    vec_int_free(&v3);
    vec_int_check_size_cap(tinfo, &v3, 0, 0);
}

void vec_int_merge_tests(tinfo* tinfo) {
    VecInt v1 = vec_int_alloc_cap(7);
    vec_int_check_size_cap(tinfo, &v1, 0, 7);
    for (int i = 1; i < 6; ++i)
        vec_int_push(&v1, i);
    vec_int_check_size_cap(tinfo, &v1, 5, 7);
    vec_int_match(tinfo, &v1, "1 2 3 4 5");

    VecInt v2 = vec_int_alloc();
    vec_int_check_size_cap(tinfo, &v2, 0, 32);
    for (int i = 6; i < 12; ++i)
        vec_int_push(&v2, i);
    vec_int_check_size_cap(tinfo, &v2, 6, 32);
    vec_int_match(tinfo, &v2, "6 7 8 9 10 11");

    vec_int_merge(&v1, &v2);
    vec_int_match(tinfo, &v1, "1 2 3 4 5 6 7 8 9 10 11");
    vec_int_check_size_cap(tinfo, &v1, 11, 11);
    vec_int_check_size_cap(tinfo, &v2, 0, 0);
    // v2 already freed by merge
    vec_int_free(&v1);
}

void vec_int_match(tinfo* tinfo, VecInt* v, char* expected) {
    char* out = vec_int_to_str(v);
    check_str_eq(tinfo, out, expected);
    free(out);
}

void vec_int_check_size_cap(tinfo* tinfo, VecInt* v, int size, int cap) {
    tinfo->total++;
    if (vec_int_size(v) != size) {
        fprintf(stderr, "FAIL: %s vec_int_size() expected %d, got %d\n",
                tinfo->tag, size, vec_int_size(v));
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_int_isempty(v) != (size == 0)) {
        fprintf(stderr,
                "FAIL: %s vec_int_isempty() expected %s, got %s size=%d\n",
                tinfo->tag, bool_to_str(size == 0),
                bool_to_str(vec_int_isempty(v)), size);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_int_cap(v) != cap) {
        fprintf(stderr, "FAIL: %s vec_int_cap() expected %d, got %d\n",
                tinfo->tag, cap, vec_int_cap(v));
    } else
        tinfo->ok++;
}

void vec_int_same(tinfo* tinfo, VecInt* v1, VecInt* v2) {
    tinfo->total++;
    if (!vec_int_equal(v1, v2)) {
        fprintf(stderr,
                "FAIL: %s vec_int_equal() expected true, got false\n",
                tinfo->tag);
    } else
        tinfo->ok++;
}
