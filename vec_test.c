// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_test.h"
#include "sx.h"
#include "tag_test.h"
#include "vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Woverride-init"
#pragma GCC diagnostic push

void vec_check_size_cap(tinfo* tinfo, const vec* v, size_t size,
                        size_t capacity);
void vec_match(tinfo* tinfo, const vec* v, const char* expected);
void vec_same(tinfo* tinfo, const vec* v1, const vec* v2);
void vec_print(const vec* v);
void vec_tests(tinfo*);
void vec_merge_tests(tinfo*);
void vec_sort_tests(tinfo*);

static bool vec_tag_find(const vec* v, const char* name, size_t* index);
static void vec_tag_sort(vec* v);
static void tags_qsort(void** array, usize low, usize high);
static int partition(void** array, usize low, usize high);
static void swap(void** array, usize i, usize j);
static bool vec_tag_search(const vec* v, const char* s, size_t* index);
static bool vec_tag_equal(const vec* v1, const vec* v2);

void vec_tests(tinfo* tinfo) {
    vec_merge_tests(tinfo);
    vec_sort_tests(tinfo);

    tag_make(true);
    vec v1 = vec_alloc(.cap = 5, .cpy = tag_copy, .destroy = tag_free);
    vec_check_size_cap(tinfo, &v1, 0, 5);

    vec v2 = vec_copy(&v1);
    vec_check_size_cap(tinfo, &v1, 0, 5);

    for (size_t i = 0; i < 7; ++i)
        vec_push(&v1, tag_make(false));
    vec_check_size_cap(tinfo, &v1, 7, 10);
    vec_match(tinfo, &v1,
              "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");
    vec_free(&v2);
    v2 = vec_copy(&v1);
    vec_check_size_cap(tinfo, &v2, 7, 7);
    vec_match(tinfo, &v2,
              "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106");
    vec_same(tinfo, &v1, &v2);
    check_bool_eq(tinfo, vec_tag_equal(&v1, &v2), true);

    Tag* t1 = vec_pop(&v1);
    check_str_eq(tinfo, t1->name, "Ag#106");
    check_int_eq(tinfo, t1->id, 106);
    tag_free(t1);

    t1 = vec_pop(&v1);
    check_str_eq(tinfo, t1->name, "Af#105");
    check_int_eq(tinfo, t1->id, 105);
    tag_free(t1);
    t1 = NULL;

    vec_check_size_cap(tinfo, &v1, 5, 10);
    vec_match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104");
    check_bool_eq(tinfo, vec_tag_equal(&v1, &v2), false);

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
    vec_check_size_cap(tinfo, &v1, 6, 10);
    vec_match(tinfo, &v1, "Ah#107|Ab#101|Ac#102|Aj#109|Ai#108|Ak#110");

    t1 = vec_replace(&v1, 0, tag_make(false));
    tag_free(t1);
    t1 = vec_replace(&v1, 4, tag_make(false));
    tag_free(t1);
    t1 = vec_replace(&v1, vec_size(&v1) - 1, tag_make(false));
    tag_free(t1);
    vec_check_size_cap(tinfo, &v1, 6, 10);
    vec_match(tinfo, &v1, "Al#111|Ab#101|Ac#102|Aj#109|Am#112|An#113");

    vec_remove(&v1, 0);
    vec_check_size_cap(tinfo, &v1, 5, 10);
    vec_match(tinfo, &v1, "Ab#101|Ac#102|Aj#109|Am#112|An#113");

    vec_remove(&v1, vec_size(&v1) - 1);
    vec_check_size_cap(tinfo, &v1, 4, 10);
    vec_match(tinfo, &v1, "Ab#101|Ac#102|Aj#109|Am#112");

    vec_remove(&v1, 1);
    vec_check_size_cap(tinfo, &v1, 3, 10);
    vec_match(tinfo, &v1, "Ab#101|Aj#109|Am#112");

    vec_remove(&v1, 0);
    vec_check_size_cap(tinfo, &v1, 2, 10);
    vec_match(tinfo, &v1, "Aj#109|Am#112");

    vec_remove(&v1, 0);
    vec_check_size_cap(tinfo, &v1, 1, 10);
    vec_match(tinfo, &v1, "Am#112");

    vec_remove(&v1, 0);
    vec_check_size_cap(tinfo, &v1, 0, 10);

    vec_clear(&v1);
    vec_check_size_cap(tinfo, &v1, 0, 10);
    vec_free(&v1);
    vec_check_size_cap(tinfo, &v1, 0, 0);
    vec_clear(&v2);
    vec_check_size_cap(tinfo, &v2, 0, 7);
    vec_free(&v2);
    vec_check_size_cap(tinfo, &v2, 0, 0);
}

void vec_merge_tests(tinfo* tinfo) {
    tag_make(true);
    vec v1 = vec_alloc(.cap = 7, .cpy = tag_copy, .destroy = tag_free);
    vec_check_size_cap(tinfo, &v1, 0, 7);
    for (size_t i = 0; i < 5; ++i)
        vec_push(&v1, tag_make(false));
    vec_check_size_cap(tinfo, &v1, 5, 7);
    vec_match(tinfo, &v1, "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104");

    vec v2 = vec_alloc(.cap = 11, .cpy = tag_copy, .destroy = tag_free);
    vec_check_size_cap(tinfo, &v2, 0, 11);
    for (size_t i = 0; i < 6; ++i)
        vec_push(&v2, tag_make(false));
    vec_check_size_cap(tinfo, &v2, 6, 11);
    vec_match(tinfo, &v2, "Af#105|Ag#106|Ah#107|Ai#108|Aj#109|Ak#110");

    vec_merge(&v1, &v2);
    vec_match(tinfo, &v1,
              "Aa#100|Ab#101|Ac#102|Ad#103|Ae#104|Af#105|Ag#106|Ah#107|Ai#"
              "108|Aj#109|Ak#110");
    vec_check_size_cap(tinfo, &v1, 11, 11);
    vec_check_size_cap(tinfo, &v2, 0, 0);

    // v2 already freed
    vec_free(&v1);
}

void vec_sort_tests(tinfo* tinfo) {
    tag_make(true);
    vec v1 = vec_alloc(.cap = 7, .cpy = tag_copy, .destroy = tag_free);
    vec_check_size_cap(tinfo, &v1, 0, 7);
    for (size_t i = 0; i < 5; ++i)
        vec_push(&v1, tag_make(false));
    vec_insert(&v1, 0, tag_alloc("Zz#999", 999));
    vec_insert(&v1, 2, tag_alloc("Ww#888", 888));
    vec_insert(&v1, 4, tag_alloc("Ae#005", 5));
    vec_insert(&v1, 6, tag_alloc("Aa#001", 1));
    vec_check_size_cap(tinfo, &v1, 9, 14);
    vec_match(
        tinfo, &v1,
        "Zz#999|Aa#100|Ww#888|Ab#101|Ae#005|Ac#102|Aa#001|Ad#103|Ae#104");

    bool found;
    size_t index;

    found = vec_tag_find(&v1, "Ae#005", &index);
    check_bool_eq(tinfo, found, true);
    check_int_eq(tinfo, index, 4);

    found = vec_tag_find(&v1, "Zz#999", &index);
    check_bool_eq(tinfo, found, true);
    check_int_eq(tinfo, index, 0);

    found = vec_tag_find(&v1, "Ae#104", &index);
    check_bool_eq(tinfo, found, true);
    check_int_eq(tinfo, index, 8);

    found = vec_tag_find(&v1, "Xy#000", &index);
    check_bool_eq(tinfo, found, false);

    vec_tag_sort(&v1);
    vec_check_size_cap(tinfo, &v1, 9, 14);
    vec_match(
        tinfo, &v1,
        "Aa#001|Aa#100|Ab#101|Ac#102|Ad#103|Ae#005|Ae#104|Ww#888|Zz#999");

    found = vec_tag_find(&v1, "Ae#005", &index);
    check_bool_eq(tinfo, found, true);
    check_int_eq(tinfo, index, 5);

    found = vec_tag_find(&v1, "Zz#999", &index);
    check_bool_eq(tinfo, found, true);
    check_int_eq(tinfo, index, 8);

    found = vec_tag_find(&v1, "Ae#104", &index);
    check_bool_eq(tinfo, found, true);
    check_int_eq(tinfo, index, 6);

    found = vec_tag_find(&v1, "Xy#000", &index);
    check_bool_eq(tinfo, found, false);

    found = vec_tag_search(&v1, "Ae#005", &index);
    check_bool_eq(tinfo, found, true);
    check_int_eq(tinfo, index, 5);

    found = vec_tag_search(&v1, "Zz#999", &index);
    check_bool_eq(tinfo, found, true);
    check_int_eq(tinfo, index, 8);

    found = vec_tag_search(&v1, "Ae#104", &index);
    check_bool_eq(tinfo, found, true);
    check_int_eq(tinfo, index, 6);

    found = vec_tag_search(&v1, "Xy#000", &index);
    check_bool_eq(tinfo, found, false);

    vec_free(&v1);
}

void vec_match(tinfo* tinfo, const vec* v, const char* expected) {
    char buf[1000];
    size_t n = 0;
    for (size_t i = 0; i < vec_size(v); i++) {
        const Tag* tag = vec_get(v, i);
        n += sprintf(&buf[n], "%s|", tag->name);
    }
    buf[n - 1] = 0;
    check_str_eq(tinfo, &buf[0], expected);
}

void vec_check_size_cap(tinfo* tinfo, const vec* v, size_t size,
                        size_t capacity) {
    tinfo->total++;
    if (vec_size(v) != size) {
        fprintf(stderr, "FAIL: %s vec_size() expected %zu, got %zu\n",
                tinfo->tag, size, vec_size(v));
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_isempty(v) != (size == 0)) {
        fprintf(stderr,
                "FAIL: %s vecy_isempty() expected %s, got %s size=%zu\n",
                tinfo->tag, bool_to_str(size == 0),
                bool_to_str(vec_isempty(v)), size);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (vec_cap(v) != capacity) {
        fprintf(stderr, "FAIL: %s vec_cap() expected %zu, got %zu\n",
                tinfo->tag, capacity, vec_cap(v));
    } else
        tinfo->ok++;
}

void vec_same(tinfo* tinfo, const vec* v1, const vec* v2) {
    tinfo->total++;
    if (!vec_tag_equal(v1, v2)) {
        fprintf(stderr, "FAIL: %s vec_equal() expected true, got false\n",
                tinfo->tag);
    } else
        tinfo->ok++;
}

bool vec_tag_find(const vec* v, const char* name, size_t* index) {
    assert_notnull(v);
    assert_notnull(name);
    assert_notnull(index);
    for (size_t i = 0; i < vec_size(v); ++i) {
        const Tag* tag = vec_get(v, i);
        if (strcmp(name, tag->name) == 0) {
            *index = i;
            return true;
        }
    }
    return false;
}

static void vec_tag_sort(vec* v) {
    assert_notnull(v);
    tags_qsort(v->_values, 0, v->_size - 1);
}

static void tags_qsort(void** array, usize low, usize high) {
    if (low < high) {
        usize partition_index = partition(array, low, high);
        tags_qsort(array, low, partition_index - 1);
        tags_qsort(array, partition_index + 1, high);
    }
}

static int partition(void** array, usize low, usize high) {
    const char* pivot = ((const Tag*)array[low])->name;
    usize i = low;
    usize j = high;
    while (i < j) {
        while (i <= high - 1 &&
               (strcmp(((const Tag*)array[i])->name, pivot) <= 0))
            i++;
        while (j >= low + 1 &&
               (strcmp(((const Tag*)array[j])->name, pivot) > 0))
            j--;
        if (i < j)
            swap(array, i, j);
    }
    swap(array, low, j);
    return j;
}

static void swap(void** array, usize i, usize j) {
    void* t = array[i];
    array[i] = array[j];
    array[j] = t;
}

static bool vec_tag_search(const vec* v, const char* s, size_t* index) {
    assert_notnull(v);
    assert_notnull(s);
    assert_notnull(index);
    if (v->_size) {
        size_t low = 0;
        size_t high = v->_size - 1;
        while (high && low <= high) {
            size_t mid = low + ((high - low) / 2);
            const Tag* tag = v->_values[mid];
            int cmp = strcmp(tag->name, s);
            if (cmp == 0) {
                *index = mid;
                return true;
            }
            if (cmp < 0)
                low = mid + 1;
            else
                high = mid - 1;
        }
    }
    return false;
}

static bool vec_tag_equal(const vec* v1, const vec* v2) {
    assert_notnull(v1);
    assert_notnull(v2);
    if (v1->_cpy != v2->_cpy || v1->_destroy != v2->_destroy ||
        v1->_size != v2->_size)
        return false;
    for (size_t i = 0; i < v1->_size; ++i) {
        if (!tag_equal(v1->_values[i], v2->_values[i]))
            return false;
    }
    return true;
}

#pragma GCC diagnostic pop
