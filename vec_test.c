// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_test.h"
#include "sx.h"
#include "tag_test.h"
#include "vec.h"
#include "vec_common_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Woverride-init"
#pragma GCC diagnostic push

static void check_size_cap(tinfo* tinfo, const Vec* v, int size, int cap);
static void match(tinfo* tinfo, const Vec* v, const char* expected);
static void same(tinfo* tinfo, const Vec* v1, const Vec* v2, bool exact);
static void merge_tests(tinfo*);
static void sort_tests(tinfo*);

void vec_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    merge_tests(tinfo);
    sort_tests(tinfo);

    tinfo->tag = "vec_tests continued";
    if (tinfo->verbose)
        puts(tinfo->tag);
    tag_make(true);
    Vec v1 = vec_alloc(.cap = 5, .cmp = tag_cmp, .cpy = tag_copy,
                       .destroy = tag_free);
    check_size_cap(tinfo, &v1, 0, 5);

    Vec v2 = vec_copy(&v1, BORROWS); // copying empty so OWNS|BORROWS ok
    check_size_cap(tinfo, &v1, 0, 5);

    for (int i = 0; i < 7; ++i)
        vec_push(&v1, tag_make(false));
    check_size_cap(tinfo, &v1, 7, 10);
    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");

    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");
    vec_free(&v2);
    v2 = vec_copy(&v1, BORROWS);
    check_size_cap(tinfo, &v2, 7, 7);
    match(tinfo, &v2, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");
    same(tinfo, &v1, &v2, false);
    check_bool_eq(tinfo, vec_equal(&v1, &v2), true);

    Vec v3 = vec_copy(&v1, BORROWS);
    check_size_cap(tinfo, &v3, 7, 7);
    match(tinfo, &v3, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");
    same(tinfo, &v1, &v3, false);
    check_bool_eq(tinfo, vec_equal(&v1, &v3), true);
    vec_free(&v3);

    check_size_cap(tinfo, &v1, 7, 10);
    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");
    match(tinfo, &v2, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");

    Tag* t1 = vec_pop(&v1);
    check_str_eq(tinfo, t1->name, "Ag#106");
    check_int_eq(tinfo, t1->id, 106);
    tag_free(t1);
    check_size_cap(tinfo, &v1, 6, 10);
    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105");
    check_bool_eq(tinfo, vec_equal(&v1, &v2), false);

    t1 = vec_get_first(&v1);
    check_str_eq(tinfo, t1->name, "Aa#100");
    check_int_eq(tinfo, t1->id, 100);

    t1 = vec_pop(&v1);
    check_str_eq(tinfo, t1->name, "Af#105");
    check_int_eq(tinfo, t1->id, 105);
    tag_free(t1);
    t1 = NULL;
    check_size_cap(tinfo, &v1, 5, 10);
    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104");
    check_bool_eq(tinfo, vec_equal(&v1, &v2), false);

    const Tag* t2 = vec_get(&v1, 0);
    check_str_eq(tinfo, t2->name, "Aa#100");
    check_int_eq(tinfo, t2->id, 100);

    t2 = vec_get_last(&v1);
    check_str_eq(tinfo, t2->name, "Ae#104");
    check_int_eq(tinfo, t2->id, 104);

    t2 = vec_get(&v1, 2);
    check_str_eq(tinfo, t2->name, "Ac#102");
    check_int_eq(tinfo, t2->id, 102);

    vec_set(&v1, 0, tag_make(false));
    vec_set(&v1, vec_size(&v1) - 1, tag_make(false));
    vec_set(&v1, 3, tag_make(false));
    vec_push(&v1, tag_make(false));
    check_size_cap(tinfo, &v1, 6, 10);
    match(tinfo, &v1, "Ah#107|Ab#101|Ac#102|Aj#109|Ai#108|Ak#110");

    t1 = vec_replace(&v1, 0, tag_make(false));
    tag_free(t1);
    t1 = vec_replace(&v1, 4, tag_make(false));
    tag_free(t1);
    t1 = vec_replace(&v1, vec_size(&v1) - 1, tag_make(false));
    tag_free(t1);
    check_size_cap(tinfo, &v1, 6, 10);
    match(tinfo, &v1, "Al#111|Ab#101|Ac#102|Aj#109|Am#112|An#113");

    vec_remove(&v1, 0);
    check_size_cap(tinfo, &v1, 5, 10);
    match(tinfo, &v1, "Ab#101|Ac#102|Aj#109|Am#112|An#113");

    vec_remove(&v1, vec_size(&v1) - 1);
    check_size_cap(tinfo, &v1, 4, 10);
    match(tinfo, &v1, "Ab#101|Ac#102|Aj#109|Am#112");

    vec_remove(&v1, 1);
    check_size_cap(tinfo, &v1, 3, 10);
    match(tinfo, &v1, "Ab#101|Aj#109|Am#112");

    vec_remove(&v1, 0);
    check_size_cap(tinfo, &v1, 2, 10);
    match(tinfo, &v1, "Aj#109|Am#112");

    vec_remove(&v1, 0);
    check_size_cap(tinfo, &v1, 1, 10);
    match(tinfo, &v1, "Am#112");

    vec_remove(&v1, 0);
    check_size_cap(tinfo, &v1, 0, 10);

    vec_clear(&v1);
    check_size_cap(tinfo, &v1, 0, 10);
    vec_free(&v1);
    check_size_cap(tinfo, &v1, 0, 0);
    vec_clear(&v2);
    check_size_cap(tinfo, &v2, 0, 7);
    vec_free(&v2);
    check_size_cap(tinfo, &v2, 0, 0);
}

static void merge_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    tag_make(true);
    Vec v1 = vec_alloc(.cap = 7, .cmp = tag_cmp, .cpy = tag_copy,
                       .destroy = tag_free);
    check_size_cap(tinfo, &v1, 0, 7);
    for (int i = 0; i < 5; ++i)
        vec_push(&v1, tag_make(false));
    check_size_cap(tinfo, &v1, 5, 7);
    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104");

    Vec v2 = vec_alloc(.cap = 11, .cmp = tag_cmp, .cpy = tag_copy,
                       .destroy = tag_free);
    check_size_cap(tinfo, &v2, 0, 11);
    for (int i = 0; i < 6; ++i)
        vec_push(&v2, tag_make(false));
    check_size_cap(tinfo, &v2, 6, 11);
    match(tinfo, &v2, "Af#105|Ag#106|Ah#107|Ai#108|Aj#109|Ak#110");

    vec_merge(&v1, &v2);
    match(tinfo, &v1,
          "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106|Ah#107|Ai#"
          "108|Aj#109|Ak#110");
    check_size_cap(tinfo, &v1, 11, 11);
    check_size_cap(tinfo, &v2, 0, 0);

    // v2 already freed by vec_merge
    vec_free(&v1);
}

static void sort_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    tag_make(true);
    Vec v1 = vec_alloc(.cap = 7, .cmp = tag_cmp, .cpy = tag_copy,
                       .destroy = tag_free);
    check_size_cap(tinfo, &v1, 0, 7);
    for (int i = 0; i < 5; ++i)
        vec_push(&v1, tag_make(false));
    vec_insert(&v1, 0, tag_alloc(strdup("Zz#999"), 999));
    vec_insert(&v1, 2, tag_alloc(strdup("Ww#888"), 888));
    vec_insert(&v1, 4, tag_alloc(strdup("Ae#005"), 5));
    vec_insert(&v1, 6, tag_alloc(strdup("Aa#001"), 1));
    check_size_cap(tinfo, &v1, 9, 14);
    match(tinfo, &v1,
          "Zz#999|Aa#100|Ww#888|Ab#101|Ae#005|Ac#102|Aa#001|Ad#103|Ae#104");

    int index;
    Tag tag = {"", 0};

    tag.name = "Ae#005";
    index = vec_find(&v1, &tag);
    check_found(tinfo, index, 4);

    tag.name = "Zz#999";
    index = vec_find(&v1, &tag);
    check_found(tinfo, index, 0);

    tag.name = "Ae#104";
    index = vec_find(&v1, &tag);
    check_found(tinfo, index, 8);

    tag.name = "Ae#104";
    index = vec_find_last(&v1, &tag);
    check_found(tinfo, index, 8);

    tag.name = "Xy#000";
    index = vec_find(&v1, &tag);
    check_found(tinfo, index, VEC_NOT_FOUND);

    tag.name = "Xy#000";
    index = vec_find_last(&v1, &tag);
    check_found(tinfo, index, VEC_NOT_FOUND);

    vec_sort(&v1);
    check_size_cap(tinfo, &v1, 9, 14);
    match(tinfo, &v1,
          "Aa#001|Aa#100|Ab#101|Ac#102|Ad#103|Ae#005|Ae#104|Ww#888|Zz#999");

    tag.name = "Ae#005";
    index = vec_find(&v1, &tag);
    check_found(tinfo, index, 5);

    tag.name = "Zz#999";
    index = vec_find(&v1, &tag);
    check_found(tinfo, index, 8);

    tag.name = "Ae#104";
    index = vec_find(&v1, &tag);
    check_found(tinfo, index, 6);

    tag.name = "Xy#000";
    index = vec_find(&v1, &tag);
    check_found(tinfo, index, VEC_NOT_FOUND);

    tag.name = "Ae#005";
    index = vec_search(&v1, &tag);
    check_found(tinfo, index, 5);

    tag.name = "Zz#999";
    index = vec_search(&v1, &tag);
    check_found(tinfo, index, 8);

    tag.name = "Ae#104";
    index = vec_search(&v1, &tag);
    check_found(tinfo, index, 6);

    tag.name = "Xy#000";
    index = vec_search(&v1, &tag);
    check_found(tinfo, index, VEC_NOT_FOUND);

    match(tinfo, &v1,
          "Aa#001|Aa#100|Ab#101|Ac#102|Ad#103|Ae#005|Ae#104|Ww#888|Zz#999");
    vec_add(&v1, tag_alloc(strdup("Aa#000"), 0));
    match(tinfo, &v1,
          "Aa#000|Aa#001|Aa#100|Ab#101|Ac#102|Ad#103|Ae#005|Ae#104|Ww#"
          "888|Zz#999");
    vec_add(&v1, tag_alloc(strdup("zz#999"), 0));
    match(tinfo, &v1,
          "Aa#000|Aa#001|Aa#100|Ab#101|Ac#102|Ad#103|Ae#005|Ae#104|Ww#"
          "888|Zz#999|zz#999");
    vec_add(&v1, tag_alloc(strdup("Af#200"), 0));
    match(tinfo, &v1,
          "Aa#000|Aa#001|Aa#100|Ab#101|Ac#102|Ad#103|Ae#005|Ae#104|"
          "Af#200|Ww#888|Zz#999|zz#999");
    vec_add(&v1, tag_alloc(strdup("Af#200"), 0));
    match(tinfo, &v1,
          "Aa#000|Aa#001|Aa#100|Ab#101|Ac#102|Ad#103|Ae#005|Ae#104|"
          "Af#200|Af#200|Ww#888|Zz#999|zz#999");

    vec_free(&v1);
}

static void match(tinfo* tinfo, const Vec* v, const char* expected) {
    char buf[1000] = {0}; // guarantee start with NUL if Vec is empty
    int n = 0;
    for (int i = 0; i < vec_size(v); ++i) {
        const Tag* tag = vec_get(v, i);
        n += sprintf(&buf[n], "%s|", tag->name);
    }
    if (n)
        buf[n - 1] = 0;
    check_str_eq(tinfo, &buf[0], expected);
}

static void check_size_cap(tinfo* tinfo, const Vec* v, int size, int cap) {
    tinfo->total++;
    if (vec_size(v) != size) {
        fprintf(stderr, "FAIL: %s vec_size() expected %d != %d\n",
                tinfo->tag, size, vec_size(v));
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_isempty(v) != (size == 0)) {
        fprintf(stderr,
                "FAIL: %s vec_isempty() expected %s != %s size=%d\n",
                tinfo->tag, bool_to_str(size == 0),
                bool_to_str(vec_isempty(v)), size);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_cap(v) != cap) {
        fprintf(stderr, "FAIL: %s vec_cap() expected %d != %d\n",
                tinfo->tag, cap, vec_cap(v));
    } else
        tinfo->ok++;
}

static void same(tinfo* tinfo, const Vec* v1, const Vec* v2, bool exact) {
    tinfo->total++;
    if (!vec_equal(v1, v2)) {
        fprintf(stderr, "FAIL: %s vec_equal() expected true != false\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    bool sm = vec_same(v1, v2);
    if (sm != exact) {
        fprintf(stderr, "FAIL: %s vec_same() expected %s != %s\n",
                tinfo->tag, bool_to_str(exact), bool_to_str(sm));
    } else
        tinfo->ok++;
}

#pragma GCC diagnostic pop
