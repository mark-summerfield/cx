// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "map_str_real_test.h"
#include "map_str_real.h"
#include <stdio.h>

static void check_size(tinfo* tinfo, MapStrReal* map, int size);

void map_str_real_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    MapStrReal vars = map_str_real_alloc(BORROWS);
    check_size(tinfo, &vars, 0);
    const int SIZE = 5;
    char* keys[] = {"one", "two", "three", "four", "five"};
    for (int i = 0; i < SIZE; ++i) {
        map_str_real_add(&vars, keys[i], (i + 1) * 10.5);
        check_size(tinfo, &vars, i + 1);
    }
    check_size(tinfo, &vars, SIZE);
    bool ok;
    double d = map_str_real_get(&vars, keys[2], &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 31.5);
    d = map_str_real_get(&vars, keys[3], &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 42);

    map_str_real_add(&vars, keys[3], 99.9);
    check_size(tinfo, &vars, SIZE);
    d = map_str_real_get(&vars, keys[3], &ok);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, d, 99.9);

    // TODO check gets
    // TODO check removes
    // TODO check adds incl. dup keys
    // TODO check to vec

    map_str_real_free(&vars);
    check_size(tinfo, &vars, 0);
}

static void check_size(tinfo* tinfo, MapStrReal* map, int size) {
    check_bool_eq(tinfo, size == 0, map_str_real_isempty(map));
    check_int_eq(tinfo, size, map_str_real_size(map));
}
