// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_int.h"
#include "cx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void vec_int_grow(vec_int* v);

vec_int vec_int_alloc_cap(int cap) {
    cap = cap ? cap : 32;
    int* values = malloc(cap * sizeof(int));
    assert_alloc(values);
    return (vec_int){._size = 0, ._cap = cap, ._values = values};
}

void vec_int_free(vec_int* v) {
    assert_notnull(v);
    free(v->_values);
    v->_values = NULL;
    v->_size = 0;
    v->_cap = 0;
}

inline void vec_int_clear(vec_int* v) { v->_size = 0; }

int vec_int_get(const vec_int* v, int index) {
    assert_notnull(v);
    assert_valid_index(v, index);
    return v->_values[index];
}

inline int vec_int_get_last(const vec_int* v) {
    assert_notnull(v);
    assert_nonempty(v);
    return v->_values[v->_size - 1];
}

void vec_int_set(vec_int* v, int index, int value) {
    assert_notnull(v);
    assert_valid_index(v, index);
    v->_values[index] = value;
}

void vec_int_insert(vec_int* v, int index, int value) {
    assert_notnull(v);
    if (index == v->_size) { // add at the end
        vec_int_push(v, value);
        return;
    }
    assert_valid_index(v, index);
    if (v->_size == v->_cap)
        vec_int_grow(v);
    for (int i = v->_size; i > index; --i)
        v->_values[i] = v->_values[i - 1];
    v->_values[index] = value;
    v->_size++;
}

void vec_int_add(vec_int* v, int value) {
    assert_notnull(v);
    assert_notnull(value);
    int high = v->_size - 1;
    if (!v->_size || v->_values[high] <= value) {
        vec_int_push(v,
                     value); // vec is empty -or- nonempty and value >= high
    } else {
        int low = 0;
        while (low < high) {
            int mid = (low + high) / 2;
            if (v->_values[mid] > value)
                high = mid;
            else
                low = mid + 1;
        }
        vec_int_insert(v, low, value);
    }
}

int vec_int_replace(vec_int* v, int index, int value) {
    assert_notnull(v);
    assert_valid_index(v, index);
    int old = v->_values[index];
    v->_values[index] = value;
    return old;
}

void vec_int_remove(vec_int* v, int index) {
    assert_notnull(v);
    assert_valid_index(v, index);
    for (int i = index; i < v->_size; ++i)
        v->_values[i] = v->_values[i + 1];
    v->_size--;
}

int vec_int_take(vec_int* v, int index) {
    assert_notnull(v);
    assert_valid_index(v, index);
    int old = v->_values[index];
    vec_int_remove(v, index);
    return old;
}

int vec_int_pop(vec_int* v) {
    assert_notnull(v);
    assert_nonempty(v);
    return v->_values[--v->_size];
}

void vec_int_push(vec_int* v, int value) {
    assert_notnull(v);
    if (v->_size == v->_cap)
        vec_int_grow(v);
    v->_values[v->_size++] = value;
}

vec_int vec_int_copy(const vec_int* v) {
    assert_notnull(v);
    vec_int vc = vec_int_alloc_cap(v->_size);
    for (int i = 0; i < v->_size; ++i)
        vec_int_push(&vc, v->_values[i]);
    return vc;
}

void vec_int_merge(vec_int* v1, vec_int* v2) {
    assert_notnull(v1);
    assert_notnull(v2);
    if ((v1->_cap - v1->_size) < v2->_size) { // v1 doesn't have enough cap
        int cap = v1->_size + v2->_size;
        int* p = realloc(v1->_values, cap * sizeof(int));
        assert_alloc(p);
        v1->_values = p;
        v1->_cap = cap;
    }
    for (int i = 0; i < v2->_size; ++i)
        v1->_values[v1->_size++] = v2->_values[i]; // push
    free(v2->_values);
    v2->_values = NULL;
    v2->_cap = 0;
    v2->_size = 0;
}

bool vec_int_equal(const vec_int* v1, const vec_int* v2) {
    assert_notnull(v1);
    assert_notnull(v2);
    if (v1->_size != v2->_size)
        return false;
    for (int i = 0; i < v1->_size; ++i)
        if (v1->_values[i] != v2->_values[i])
            return false;
    return true;
}

int vec_int_find(const vec_int* v, int value) {
    assert_notnull(v);
    for (int i = 0; i < v->_size; ++i)
        if (v->_values[i] == value)
            return i;
    return VEC_NOT_FOUND;
}

int vec_int_find_last(const vec_int* v, int value) {
    assert_notnull(v);
    for (int i = v->_size - 1; i >= 0; --i)
        if (v->_values[i] == value)
            return i;
    return VEC_NOT_FOUND;
}

int intcmp(const void* a, const void* b) { return (*(int*)a - *(int*)b); }

void vec_int_sort(vec_int* v) {
    assert_notnull(v);
    if (v->_size)
        qsort(v->_values, v->_size, sizeof(int), intcmp);
}

int vec_int_search(const vec_int* v, int i) {
    assert_notnull(v);
    if (v->_size) {
        const int* p =
            bsearch(&i, v->_values, v->_size, sizeof(int), intcmp);
        if (p)
            return p - v->_values;
    }
    return VEC_NOT_FOUND;
}

char* vec_int_tostring(const vec_int* v) {
    assert_notnull(v);
    const int BUF_SIZE = 128;
    const int VEC_SIZE = vec_int_size(v);
    int cap = VEC_SIZE * 4;
    char* s = malloc(cap);
    assert_alloc(s);
    int pos = 0;
    char buf[BUF_SIZE];
    for (int i = 0; i < VEC_SIZE; ++i) {
        size_t n = snprintf(buf, BUF_SIZE, "%d ", vec_int_get(v, i));
        strncpy(&s[pos], buf, n);
        pos += n;
        if (pos + 4 > cap) {
            cap *= 2;
            char* p = realloc(s, cap);
            assert_alloc(p);
            s = p;
        }
    }
    s[pos - 1] = 0; // avoid trailing space
    return s;
}

static void vec_int_grow(vec_int* v) {
    const int BLOCK_SIZE = 1024 * 1024;
    int cap = (v->_cap < BLOCK_SIZE) ? v->_cap * 2 : v->_cap + BLOCK_SIZE;
    int* p = realloc(v->_values, cap * sizeof(int));
    assert_alloc(p);
    v->_values = p;
    v->_cap = cap;
}
