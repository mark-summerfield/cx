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
    /*

    vec v1 = vec_alloc(); // default of 32
    vec_check_size_cap(tinfo, &v1, 0, 32);

    vec v2 = vec_copy(&v1);
    vec_check_size_cap(tinfo, &v1, 0, 32);

    tinfo->total++;
    int WORD_COUNT = sizeof(WORDS) / sizeof(char*);
    for (int i = 0; i < WORD_COUNT; ++i) {
        vec_check_size_cap(tinfo, &v1, i, i <= 32 ? 32 : 64);
        vec_push(&v1, strdup(WORDS[i]));
        if (i < 10) {
            vec_check_size_cap(tinfo, &v2, i, 32);
            vec_push(&v2, strdup(WORDS[i]));
        }
    }
    tinfo->ok++;

    vec_check_size_cap(tinfo, &v1, WORD_COUNT, 64);
    vec_check_size_cap(tinfo, &v2, 10, 32);
    const char* V2 =
        "One|Zulu|Victor|Romeo|Sierra|Whiskey|X-ray|Two|India|Papa";
    vec_match(tinfo, &v2, V2);

    for (int i = WORD_COUNT - 1; i > 9; --i) {
        char* s = vec_pop(&v1);
        check_eq(tinfo, s, WORDS[i]);
        free(s);
    }
    vec_match(tinfo, &v1, V2);
    vec_check_size_cap(tinfo, &v1, 10, 64);
    vec_check_size_cap(tinfo, &v2, 10, 32);
    vec_same(tinfo, &v1, &v2);

    vec_push(&v1, strdup("alpha"));
    const char* s1 = vec_get_last(&v1);
    check_eq(tinfo, s1, "alpha");
    vec_match(
        tinfo, &v1,
        "One|Zulu|Victor|Romeo|Sierra|Whiskey|X-ray|Two|India|Papa|alpha");

    vec_insert(&v1, 4, strdup("beta"));
    const char* s2 = vec_get(&v1, 4);
    check_eq(tinfo, s2, "beta");
    vec_match(tinfo, &v1,
                  "One|Zulu|Victor|Romeo|beta|Sierra|Whiskey|X-ray|Two|"
                  "India|Papa|alpha");

    const char* s0 = vec_get(&v1, 0);
    check_eq(tinfo, s0, "One");
    const char* s5 = vec_get(&v1, 5);
    check_eq(tinfo, s5, "Sierra");
    const char* s6 = vec_get(&v1, 6);
    check_eq(tinfo, s6, "Whiskey");

    vec_set(&v1, 6, strdup("gamma"));
    const char* s6a = vec_get(&v1, 6);
    check_eq(tinfo, s6a, "gamma");
    vec_match(tinfo, &v1,
                  "One|Zulu|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_set(&v1, 0, strdup("A0"));
    const char* s0a = vec_get(&v1, 0);
    check_eq(tinfo, s0a, "A0");
    vec_match(tinfo, &v1,
                  "A0|Zulu|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_insert(&v1, 2, strdup("B2"));
    const char* s2a = vec_get(&v1, 2);
    check_eq(tinfo, s2a, "B2");
    vec_match(tinfo, &v1,
                  "A0|Zulu|B2|Victor|Romeo|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_remove(&v1, 4);
    vec_match(tinfo, &v1,
                  "A0|Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_remove(&v1, 0);
    vec_match(tinfo, &v1,
                  "Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa|alpha");

    vec_remove(&v1, vec_size(&v1) - 1);
    vec_match(tinfo, &v1,
                  "Zulu|B2|Victor|beta|Sierra|gamma|X-ray|Two|"
                  "India|Papa");

    vec_replace(&v1, 3, strdup("Hairy"));
    vec_match(tinfo, &v1,
                  "Zulu|B2|Victor|Hairy|Sierra|gamma|X-ray|Two|"
                  "India|Papa");
    char* x = vec_take(&v1, 1);
    check_eq(tinfo, x, "B2");
    free(x);
    vec_match(tinfo, &v1,
                  "Zulu|Victor|Hairy|Sierra|gamma|X-ray|Two|India|Papa");

    vec_found_index found_index = vec_find(&v1, "Two");
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 6);
    found_index = vec_find(&v1, "two");
    check_bool_eq(tinfo, found_index.found, false);
    found_index = vec_find(&v1, "Papa");
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 8);
    found_index = vec_find(&v1, "Zulu");
    check_bool_eq(tinfo, found_index.found, true);
    check_int_eq(tinfo, found_index.index, 0);

    vec_clear(&v1);
    vec_check_size_cap(tinfo, &v1, 0, 64);
    vec_push(&v1, strdup("more"));
    const char* more = vec_get(&v1, 0);
    check_eq(tinfo, more, "more");
    vec_match(tinfo, &v1, "more");
    vec_check_size_cap(tinfo, &v1, 1, 64);
    vec_free(&v1);
    vec_check_size_cap(tinfo, &v1, 0, 0);
    vec_clear(&v2);
    vec_check_size_cap(tinfo, &v2, 0, 32);
    vec_free(&v2);
    vec_check_size_cap(tinfo, &v2, 0, 0);

    vec v4 = vec_alloc_split("one\ttwo\tthree\tfour\tfive", "\t");
    vec_match(tinfo, &v4, "one|two|three|four|five");
    vec_free(&v4);
    vec v5 =
        vec_alloc_split("oneSEPtwoSEPthreeSEPfourSEPfiveSEPsix", "SEP");
    vec_match(tinfo, &v5, "one|two|three|four|five|six");
    vec_free(&v5);
    */
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
