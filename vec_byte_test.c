// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_byte_test.h"
#include "exit.h"
#include "str.h"
#include "vec_byte.h"
#include "vecs_test.h"
#include <stdlib.h>

static void check_size_cap(tinfo* tinfo, VecByte* v, int size, int cap);
static void match(tinfo* tinfo, VecByte* v, char* expected);
static void equal(tinfo* tinfo, VecByte* v1, VecByte* v2);
static void merge_tests(tinfo* tinfo);

void vec_byte_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    merge_tests(tinfo);

    VecByte v1 = vec_byte_alloc(); // default of 0
    check_size_cap(tinfo, &v1, 0, 0);

    VecByte v2 = vec_byte_copy(&v1);
    check_size_cap(tinfo, &v1, 0, 0);

    tinfo->total++;
    for (int i = 1; i <= 35; ++i) {
        vec_byte_push(&v1, i);
        if (i < 10)
            vec_byte_push(&v2, i);
        int size = i <= VEC_INITIAL_CAP     ? VEC_INITIAL_CAP
                   : i <= VEC_INITIAL_CAPx2 ? VEC_INITIAL_CAPx2
                                            : VEC_INITIAL_CAPx4;
        check_size_cap(tinfo, &v1, i, size);
    }
    tinfo->ok++;

    check_size_cap(tinfo, &v1, 35, VEC_INITIAL_CAPx4);
    check_size_cap(tinfo, &v2, 9, VEC_INITIAL_CAP);
    match(tinfo, &v1,
          "01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 "
          "14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23");
    match(tinfo, &v2, "01 02 03 04 05 06 07 08 09");

    for (int i = 35; i > 9; --i) {
        int v = vec_byte_pop(&v1);
        check_int_eq(tinfo, v, i);
    }
    match(tinfo, &v1, "01 02 03 04 05 06 07 08 09");
    equal(tinfo, &v1, &v2);

    vec_byte_push(&v1, 0x63);
    match(tinfo, &v1, "01 02 03 04 05 06 07 08 09 63");

    vec_byte_insert(&v1, 4, 0x37);
    match(tinfo, &v1, "01 02 03 04 37 05 06 07 08 09 63");

    vec_byte_insert(&v1, 0, 0x21);
    match(tinfo, &v1, "21 01 02 03 04 37 05 06 07 08 09 63");

    vec_byte_insert(&v1, 0, 0x17);
    match(tinfo, &v1, "17 21 01 02 03 04 37 05 06 07 08 09 63");

    VecByte v3 = vec_byte_copy(&v1);
    check_size_cap(tinfo, &v3, VEC_SIZE(&v1), VEC_SIZE(&v1));

    int x = vec_byte_get(&v1, 0);
    check_int_eq(tinfo, x, 0x17);
    x = VEC_GET(&v1, 0);
    check_int_eq(tinfo, x, 0x17);
    x = vec_byte_get_first(&v1);
    check_int_eq(tinfo, x, 0x17);
    x = VEC_GET_FIRST(&v1);
    check_int_eq(tinfo, x, 0x17);
    x = vec_byte_get(&v1, 4);
    check_int_eq(tinfo, x, 3);
    x = VEC_GET(&v1, 4);
    check_int_eq(tinfo, x, 3);
    x = vec_byte_get_last(&v1);
    check_int_eq(tinfo, x, 0x63);
    x = VEC_GET_LAST(&v1);
    check_int_eq(tinfo, x, 0x63);

    vec_byte_remove(&v1, 1);
    match(tinfo, &v1, "17 01 02 03 04 37 05 06 07 08 09 63");

    vec_byte_remove(&v1, 0);
    match(tinfo, &v1, "01 02 03 04 37 05 06 07 08 09 63");

    vec_byte_set(&v1, 2, 0x33);
    match(tinfo, &v1, "01 02 33 04 37 05 06 07 08 09 63");
    vec_byte_set(&v1, 10, 0x10);
    match(tinfo, &v1, "01 02 33 04 37 05 06 07 08 09 10");
    vec_byte_set(&v1, 0, 0);
    match(tinfo, &v1, "00 02 33 04 37 05 06 07 08 09 10");

    x = vec_byte_replace(&v1, 4, 0xFE);
    check_int_eq(tinfo, x, 0x37);
    match(tinfo, &v1, "00 02 33 04 FE 05 06 07 08 09 10");

    vec_byte_remove(&v1, 7);
    match(tinfo, &v1, "00 02 33 04 FE 05 06 08 09 10");
    vec_byte_remove(&v1, 7);
    match(tinfo, &v1, "00 02 33 04 FE 05 06 09 10");

    x = vec_byte_take(&v1, 0);
    check_int_eq(tinfo, x, 0);
    match(tinfo, &v1, "02 33 04 FE 05 06 09 10");
    x = vec_byte_take(&v1, 7);
    check_int_eq(tinfo, x, 0x10);
    match(tinfo, &v1, "02 33 04 FE 05 06 09");

    int index;
    index = vec_byte_find(&v1, 8);
    check_found(tinfo, index, VEC_NOT_FOUND);

    index = vec_byte_find_last(&v1, 8);
    check_found(tinfo, index, VEC_NOT_FOUND);

    index = vec_byte_find(&v1, 2);
    check_found(tinfo, index, 0);

    index = vec_byte_find(&v1, 0xFE);
    check_found(tinfo, index, 3);

    index = vec_byte_find_last(&v1, 0xFE);
    check_found(tinfo, index, 3);

    index = vec_byte_find(&v1, 9);
    check_found(tinfo, index, 6);

    vec_byte_clear(&v1);
    check_size_cap(tinfo, &v1, 0, VEC_INITIAL_CAPx4);
    vec_byte_push(&v1, 0x19);
    match(tinfo, &v1, "19");
    vec_byte_free(&v1);
    check_size_cap(tinfo, &v1, 0, 0);
    vec_byte_clear(&v2);
    check_size_cap(tinfo, &v2, 0, VEC_INITIAL_CAP);
    vec_byte_free(&v2);
    check_size_cap(tinfo, &v2, 0, 0);
    vec_byte_free(&v3);
    check_size_cap(tinfo, &v3, 0, 0);
}

static void merge_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    VecByte v1 = vec_byte_alloc_cap(7);
    check_size_cap(tinfo, &v1, 0, 7);
    for (int i = 1; i < 6; ++i)
        vec_byte_push(&v1, i);
    check_size_cap(tinfo, &v1, 5, 7);
    match(tinfo, &v1, "01 02 03 04 05");

    VecByte v2 = vec_byte_alloc();
    check_size_cap(tinfo, &v2, 0, 0);
    for (int i = 6; i < 12; ++i)
        vec_byte_push(&v2, i);
    check_size_cap(tinfo, &v2, 6, VEC_INITIAL_CAP);
    match(tinfo, &v2, "06 07 08 09 0A 0B");

    vec_byte_merge(&v1, &v2);
    match(tinfo, &v1, "01 02 03 04 05 06 07 08 09 0A 0B");
    check_size_cap(tinfo, &v1, 11, 11);
    check_size_cap(tinfo, &v2, 0, 0);
    // v2 already freed by merge
    vec_byte_free(&v1);
}

static void match(tinfo* tinfo, VecByte* v, char* expected) {
    char* out = vec_byte_to_str(v);
    check_str_eq(tinfo, out, expected);
    free(out);
}

static void check_size_cap(tinfo* tinfo, VecByte* v, int size, int cap) {
    tinfo->total++;
    if (VEC_SIZE(v) != size)
        WARN("FAIL: %s VEC_SIZE() expected %d != %d\n", tinfo->tag,
             size, VEC_SIZE(v));
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

static void equal(tinfo* tinfo, VecByte* v1, VecByte* v2) {
    tinfo->total++;
    if (!vec_byte_equal(v1, v2))
        WARN("FAIL: %s vec_byte_equal() expected true != false\n",
             tinfo->tag);
    else
        tinfo->ok++;
}
