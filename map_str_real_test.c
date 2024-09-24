// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "map_str_real_test.h"
#include "map_str_real.h"
#include <stdio.h>
#include <string.h>

static void test1(tinfo* tinfo);
static void test2(tinfo* tinfo);
static void check_size(tinfo* tinfo, const MapStrReal* map, int size);
static void check_pairs(tinfo* tinfo, const MapStrReal* map);

void map_str_real_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    test1(tinfo);
    test2(tinfo);
}

static void test1(tinfo* tinfo) {
    MapStrReal vars = map_str_real_alloc(BORROWS);
    check_size(tinfo, &vars, 0);

    bool ok = map_str_real_contains(&vars, "zero");
    check_bool_eq(tinfo, ok, false);
    map_str_real_set(&vars, "zero", 2.3);
    check_size(tinfo, &vars, 1);
    double d = map_str_real_get(&vars, "zero", &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 2.3);
    map_str_real_set(&vars, "zero", 5.7);
    check_size(tinfo, &vars, 1);
    d = map_str_real_get(&vars, "zero", &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 5.7);
    map_str_real_get(&vars, "ZERO", &ok);
    check_bool_eq(tinfo, ok, false);
    ok = map_str_real_contains(&vars, "ZERO");
    check_bool_eq(tinfo, ok, false);
    ok = map_str_real_contains(&vars, "zero");
    check_bool_eq(tinfo, ok, true);

    check_pairs(tinfo, &vars);

    const int SIZE = 5;
    char* keys[] = {"one", "two", "three", "four", "five"};
    for (int i = 0; i < SIZE; ++i) {
        ok = map_str_real_contains(&vars, keys[i]);
        check_bool_eq(tinfo, ok, false);
        map_str_real_set(&vars, keys[i], (i + 1) * 10.5);
        check_size(tinfo, &vars, i + 2);
        ok = map_str_real_contains(&vars, keys[i]);
        check_bool_eq(tinfo, ok, true);
    }
    check_size(tinfo, &vars, SIZE + 1);
    d = map_str_real_get(&vars, keys[2], &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 31.5);
    d = map_str_real_get(&vars, keys[3], &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 42);

    check_pairs(tinfo, &vars);

    map_str_real_set(&vars, keys[3], 99.9);
    check_size(tinfo, &vars, SIZE + 1);
    d = map_str_real_get(&vars, keys[3], &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 99.9);

    check_pairs(tinfo, &vars);

    ok = map_str_real_remove(&vars, "XYZ");
    check_bool_eq(tinfo, ok, false);
    d = map_str_real_get(&vars, "zero", &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 5.7);
    ok = map_str_real_remove(&vars, "zero");
    check_bool_eq(tinfo, ok, true);
    map_str_real_get(&vars, "zero", &ok);
    check_bool_eq(tinfo, ok, false);

    check_pairs(tinfo, &vars);

    map_str_real_free(&vars);
    check_size(tinfo, &vars, 0);
}

static void test2(tinfo* tinfo) {
    MapStrReal vars = map_str_real_alloc(OWNS);
    check_size(tinfo, &vars, 0);

    bool ok = map_str_real_contains(&vars, "zero");
    check_bool_eq(tinfo, ok, false);
    map_str_real_set(&vars, strdup("zero"), 2.3);
    check_size(tinfo, &vars, 1);
    double d = map_str_real_get(&vars, "zero", &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 2.3);
    map_str_real_set(&vars, strdup("zero"), 5.7);
    check_size(tinfo, &vars, 1);
    d = map_str_real_get(&vars, "zero", &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 5.7);
    map_str_real_get(&vars, "ZERO", &ok);
    check_bool_eq(tinfo, ok, false);
    ok = map_str_real_contains(&vars, "ZERO");
    check_bool_eq(tinfo, ok, false);
    ok = map_str_real_contains(&vars, "zero");
    check_bool_eq(tinfo, ok, true);

    check_pairs(tinfo, &vars);

    const int SIZE = 5;
    char* keys[] = {"one", "two", "three", "four", "five"};
    for (int i = 0; i < SIZE; ++i) {
        ok = map_str_real_contains(&vars, keys[i]);
        check_bool_eq(tinfo, ok, false);
        map_str_real_set(&vars, strdup(keys[i]), (i + 1) * 10.5);
        check_size(tinfo, &vars, i + 2);
        ok = map_str_real_contains(&vars, keys[i]);
        check_bool_eq(tinfo, ok, true);
    }
    check_size(tinfo, &vars, SIZE + 1);
    d = map_str_real_get(&vars, keys[2], &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 31.5);
    d = map_str_real_get(&vars, keys[3], &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 42);

    check_pairs(tinfo, &vars);

    map_str_real_set(&vars, strdup(keys[3]), 99.9);
    check_size(tinfo, &vars, SIZE + 1);
    d = map_str_real_get(&vars, keys[3], &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 99.9);

    check_pairs(tinfo, &vars);

    ok = map_str_real_remove(&vars, "XYZ");
    check_bool_eq(tinfo, ok, false);
    d = map_str_real_get(&vars, "zero", &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 5.7);
    ok = map_str_real_remove(&vars, "zero");
    check_bool_eq(tinfo, ok, true);
    map_str_real_get(&vars, "zero", &ok);
    check_bool_eq(tinfo, ok, false);

    check_pairs(tinfo, &vars);

    map_str_real_free(&vars);
    check_size(tinfo, &vars, 0);
}

static void check_size(tinfo* tinfo, const MapStrReal* map, int size) {
    check_bool_eq(tinfo, size == 0, map_str_real_isempty(map));
    check_int_eq(tinfo, size, map_str_real_size(map));
}

static void check_pairs(tinfo* tinfo, const MapStrReal* map) {
    Vec pairs = map_str_real_to_vec(map);
    VecStr keys = map_str_real_keys(map);
    check_int_eq(tinfo, VEC_SIZE(&pairs), map_str_real_size(map));
    check_int_eq(tinfo, VEC_SIZE(&keys), map_str_real_size(map));
    bool ok;
    for (int i = 0; i < VEC_SIZE(&pairs); ++i) {
        const StrRealPair* pair = vec_get(&pairs, i);
        double d = map_str_real_get(map, pair->key, &ok);
        check_bool_eq(tinfo, ok, true);
        check_real_eq(tinfo, pair->value, d);
        check_str_eq(tinfo, pair->key, vec_str_get(&keys, i));
    }
    vec_str_free(&keys);
    vec_free(&pairs);
}
