// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3

#include "vec_test.h"
#include "exit.h"
#include "str.h"
#include "tag_test.h"
#include "vec.h"
#include "vecs_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Woverride-init"
#pragma GCC diagnostic push

static void check_size_cap(tinfo* tinfo, const Vec* v, int size, int cap);
static void match(tinfo* tinfo, const Vec* v, const char* expected);
static void equal(tinfo* tinfo, const Vec* v1, const Vec* v2, bool same);
static void merge_tests(tinfo*);
static void sort_tests(tinfo*);
static void misc_tests(tinfo* tinfo);
static void misc_test2(tinfo* tinfo, const Vec* v1);

void vec_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    merge_tests(tinfo);
    sort_tests(tinfo);
    misc_tests(tinfo);
}

static void misc_tests(tinfo* tinfo) {
    tinfo->tag = "vec_tests misc_tests";
    if (tinfo->verbose)
        puts(tinfo->tag);
    tag_make(true);
    Vec v1 = vec_alloc(5, tag_cmp, tag_free);
    check_size_cap(tinfo, &v1, 0, 5);
    check_bool_eq(tinfo, vec_ownership(&v1), Owns);

    Vec v2 = vec_copy(&v1, tag_copy);
    check_bool_eq(tinfo, vec_ownership(&v2), Owns);

    check_size_cap(tinfo, &v1, 0, 5);
    check_size_cap(tinfo, &v2, 0, 0);

    for (int i = 0; i < 7; ++i)
        vec_push(&v1, tag_make(false));
    check_size_cap(tinfo, &v1, 7, 10);
    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");

    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");
    vec_free(&v2);

    v2 = vec_copy(&v1, tag_copy);
    check_bool_eq(tinfo, vec_ownership(&v2), Owns);
    check_size_cap(tinfo, &v2, 7, 7);
    match(tinfo, &v2, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");
    equal(tinfo, &v1, &v2, true);
    check_bool_eq(tinfo, vec_equal(&v1, &v2), true);

    misc_test2(tinfo, &v1);

    check_size_cap(tinfo, &v1, 7, 10);
    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");
    match(tinfo, &v2, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");

    Tag* t1 = vec_pop(&v1);
    check_str_eq(tinfo, t1->name, "Ag#106");
    check_int_eq(tinfo, t1->id, 106);
    tag_free(t1);
    free(t1);
    check_size_cap(tinfo, &v1, 6, 10);
    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105");
    check_bool_eq(tinfo, vec_equal(&v1, &v2), false);

    t1 = vec_get_first(&v1);
    check_str_eq(tinfo, t1->name, "Aa#100");
    check_int_eq(tinfo, t1->id, 100);
    t1 = VEC_GET_FIRST(&v1);
    check_str_eq(tinfo, t1->name, "Aa#100");
    check_int_eq(tinfo, t1->id, 100);

    t1 = vec_pop(&v1);
    check_str_eq(tinfo, t1->name, "Af#105");
    check_int_eq(tinfo, t1->id, 105);
    tag_free(t1);
    free(t1);
    t1 = NULL;
    check_size_cap(tinfo, &v1, 5, 10);
    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104");
    check_bool_eq(tinfo, vec_equal(&v1, &v2), false);

    const Tag* t2 = vec_get(&v1, 0);
    check_str_eq(tinfo, t2->name, "Aa#100");
    check_int_eq(tinfo, t2->id, 100);
    t2 = VEC_GET(&v1, 0);
    check_str_eq(tinfo, t2->name, "Aa#100");
    check_int_eq(tinfo, t2->id, 100);

    t2 = vec_get_last(&v1);
    check_str_eq(tinfo, t2->name, "Ae#104");
    check_int_eq(tinfo, t2->id, 104);
    t2 = VEC_GET_LAST(&v1);
    check_str_eq(tinfo, t2->name, "Ae#104");
    check_int_eq(tinfo, t2->id, 104);

    t2 = vec_get(&v1, 2);
    check_str_eq(tinfo, t2->name, "Ac#102");
    check_int_eq(tinfo, t2->id, 102);
    t2 = VEC_GET(&v1, 2);
    check_str_eq(tinfo, t2->name, "Ac#102");
    check_int_eq(tinfo, t2->id, 102);

    vec_set(&v1, 0, tag_make(false));
    vec_set(&v1, VEC_SIZE(&v1) - 1, tag_make(false));
    vec_set(&v1, 3, tag_make(false));
    vec_push(&v1, tag_make(false));
    check_size_cap(tinfo, &v1, 6, 10);
    match(tinfo, &v1, "Ah#107|Ab#101|Ac#102|Aj#109|Ai#108|Ak#110");

    t1 = vec_replace(&v1, 0, tag_make(false));
    tag_free(t1);
    free(t1);
    t1 = vec_replace(&v1, 4, tag_make(false));
    tag_free(t1);
    free(t1);
    t1 = vec_replace(&v1, VEC_SIZE(&v1) - 1, tag_make(false));
    tag_free(t1);
    free(t1);
    check_size_cap(tinfo, &v1, 6, 10);
    match(tinfo, &v1, "Al#111|Ab#101|Ac#102|Aj#109|Am#112|An#113");

    vec_remove(&v1, 0);
    check_size_cap(tinfo, &v1, 5, 10);
    match(tinfo, &v1, "Ab#101|Ac#102|Aj#109|Am#112|An#113");

    vec_remove(&v1, VEC_SIZE(&v1) - 1);
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

static void misc_test2(tinfo* tinfo, const Vec* v1) {
    Vec v3 = vec_copy(v1, tag_copy);
    check_bool_eq(tinfo, vec_ownership(&v3), Owns);
    check_size_cap(tinfo, &v3, 7, 7);
    match(tinfo, &v3, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");
    equal(tinfo, v1, &v3, true);
    check_bool_eq(tinfo, vec_equal(v1, &v3), true);
    vec_free(&v3);
}

static void merge_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    tag_make(true);
    Vec v1 = vec_alloc(7, tag_cmp, tag_free);
    check_bool_eq(tinfo, vec_ownership(&v1), Owns);
    check_size_cap(tinfo, &v1, 0, 7);
    for (int i = 0; i < 5; ++i)
        vec_push(&v1, tag_make(false));
    check_size_cap(tinfo, &v1, 5, 7);
    match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104");

    Vec v2 = vec_alloc(11, tag_cmp, tag_free);
    check_bool_eq(tinfo, vec_ownership(&v2), Owns);
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
    check_size_cap(tinfo, &v1, 0, 0);
    check_size_cap(tinfo, &v2, 0, 0);
}

static void sort_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    tag_make(true);
    Vec v1 = vec_alloc(7, tag_cmp, tag_free);
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
    Tag tag = tag_make_key("Ae#005");

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
    for (int i = 0; i < VEC_SIZE(v); ++i) {
        const Tag* tag = vec_get(v, i);
        n += sprintf(&buf[n], "%s|", tag->name);
    }
    if (n)
        buf[n - 1] = 0;
    check_str_eq(tinfo, &buf[0], expected);
}

static void check_size_cap(tinfo* tinfo, const Vec* v, int size, int cap) {
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

static void equal(tinfo* tinfo, const Vec* v1, const Vec* v2, bool same) {
    tinfo->total++;
    if (!vec_equal(v1, v2))
        WARN("FAIL: %s vec_equal() true != false\n", tinfo->tag);
    else
        tinfo->ok++;
    tinfo->total++;
    if (same != vec_same(v1, v2))
        WARN("FAIL: %s vec_same() %s != %s\n", tinfo->tag,
             bool_to_str(same), bool_to_str(!same));
    else
        tinfo->ok++;
}

#pragma GCC diagnostic pop
