// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_int.h"
#include "cx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void vec_int_grow(VecInt* vec);
static int intcmp(const void* a, const void* b);

VecInt vec_int_alloc_cap(int cap) {
    cap = cap > 0 ? cap : 0;
    int* values = NULL;
    if (cap) {
        values = malloc(cap * sizeof(int));
        assert_alloc(values);
    }
    return (VecInt){._size = 0, ._cap = cap, ._values = values};
}

void vec_int_free(VecInt* vec) {
    assert_notnull(vec);
    free(vec->_values);
    vec->_values = NULL;
    vec->_size = 0;
    vec->_cap = 0;
}

inline void vec_int_clear(VecInt* vec) { vec->_size = 0; }

int vec_int_get(const VecInt* vec, int index) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    return vec->_values[index];
}

inline int vec_int_get_first(const VecInt* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->_values[0];
}

inline int vec_int_get_last(const VecInt* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->_values[vec->_size - 1];
}

void vec_int_set(VecInt* vec, int index, int value) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    vec->_values[index] = value;
}

void vec_int_insert(VecInt* vec, int index, int value) {
    assert_notnull(vec);
    assert_valid_index(vec, index);
    if (vec->_size == vec->_cap)
        vec_int_grow(vec);
    for (int i = vec->_size; i > index; --i)
        vec->_values[i] = vec->_values[i - 1];
    vec->_values[index] = value;
    vec->_size++;
}

void vec_int_add(VecInt* vec, int value) {
    assert_notnull(vec);
    assert_notnull(value);
    int high = vec->_size - 1;
    if (!vec->_size || vec->_values[high] <= value) {
        vec_int_push(vec,
                     value); // vec is empty -or- nonempty and value >= high
    } else {
        int low = 0;
        while (low < high) {
            int mid = (low + high) / 2;
            if (vec->_values[mid] > value)
                high = mid;
            else
                low = mid + 1;
        }
        vec_int_insert(vec, low, value);
    }
}

int vec_int_replace(VecInt* vec, int index, int value) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    int old = vec->_values[index];
    vec->_values[index] = value;
    return old;
}

void vec_int_remove(VecInt* vec, int index) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    for (int i = index; i < vec->_size; ++i)
        vec->_values[i] = vec->_values[i + 1];
    vec->_size--;
}

int vec_int_take(VecInt* vec, int index) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    int old = vec->_values[index];
    vec_int_remove(vec, index);
    return old;
}

int vec_int_pop(VecInt* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->_values[--vec->_size];
}

void vec_int_push(VecInt* vec, int value) {
    assert_notnull(vec);
    if (vec->_size == vec->_cap)
        vec_int_grow(vec);
    vec->_values[vec->_size++] = value;
}

VecInt vec_int_copy(const VecInt* vec) {
    assert_notnull(vec);
    VecInt out = vec_int_alloc_cap(vec->_size);
    for (int i = 0; i < vec->_size; ++i)
        vec_int_push(&out, vec->_values[i]);
    return out;
}

void vec_int_merge(VecInt* vec1, VecInt* vec2) {
    assert_notnull(vec1);
    assert_notnull(vec2);
    if ((vec1->_cap - vec1->_size) <
        vec2->_size) { // vec1 doesn't have enough cap
        int cap = vec1->_size + vec2->_size;
        int* p = realloc(vec1->_values, cap * sizeof(int));
        assert_alloc(p);
        vec1->_values = p;
        vec1->_cap = cap;
    }
    for (int i = 0; i < vec2->_size; ++i)
        vec1->_values[vec1->_size++] = vec2->_values[i]; // push
    vec_int_free(vec2);
}

bool vec_int_equal(const VecInt* vec1, const VecInt* vec2) {
    assert_notnull(vec1);
    assert_notnull(vec2);
    if (vec1->_size != vec2->_size)
        return false;
    for (int i = 0; i < vec1->_size; ++i)
        if (vec1->_values[i] != vec2->_values[i])
            return false;
    return true;
}

int vec_int_find(const VecInt* vec, int value) {
    assert_notnull(vec);
    for (int i = 0; i < vec->_size; ++i)
        if (vec->_values[i] == value)
            return i;
    return VEC_NOT_FOUND;
}

int vec_int_find_last(const VecInt* vec, int value) {
    assert_notnull(vec);
    for (int i = vec->_size - 1; i >= 0; --i)
        if (vec->_values[i] == value)
            return i;
    return VEC_NOT_FOUND;
}

static int intcmp(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void vec_int_sort(VecInt* vec) {
    assert_notnull(vec);
    if (vec->_size)
        qsort(vec->_values, vec->_size, sizeof(int), intcmp);
}

int vec_int_search(const VecInt* vec, int i) {
    assert_notnull(vec);
    if (vec->_size) {
        const int* p =
            bsearch(&i, vec->_values, vec->_size, sizeof(int), intcmp);
        if (p)
            return p - vec->_values;
    }
    return VEC_NOT_FOUND;
}

char* vec_int_to_str(const VecInt* vec) {
    assert_notnull(vec);
    const int BUF_SIZE = 128;
    const int VEC_SIZE = vec_int_size(vec);
    int cap = VEC_SIZE * 4;
    char* s = malloc(cap);
    assert_alloc(s);
    int pos = 0;
    char buf[BUF_SIZE];
    for (int i = 0; i < VEC_SIZE; ++i) {
        size_t n = snprintf(buf, BUF_SIZE, "%d ", vec_int_get(vec, i));
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

static void vec_int_grow(VecInt* vec) {
    int cap = GROW_CAP(vec->_cap);
    int* p = realloc(vec->_values, cap * sizeof(int));
    assert_alloc(p);
    vec->_values = p;
    vec->_cap = cap;
}
