// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_int.h"
#include "cx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void vec_int_grow(vec_int* v);

vec_int vec_int_alloc_cap(SSIZE_T cap) {
    cap = cap ? cap : 32;
    VEC_INT_VALUE_T* values = malloc(cap * sizeof(VEC_INT_VALUE_T));
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

VEC_INT_VALUE_T vec_int_get(const vec_int* v, SSIZE_T index) {
    assert_notnull(v);
    assert_valid_index(v, index);
    return v->_values[index];
}

inline VEC_INT_VALUE_T vec_int_get_last(const vec_int* v) {
    assert_notnull(v);
    assert_nonempty(v);
    return v->_values[v->_size - 1];
}

void vec_int_set(vec_int* v, SSIZE_T index, VEC_INT_VALUE_T value) {
    assert_notnull(v);
    assert_valid_index(v, index);
    v->_values[index] = value;
}

void vec_int_insert(vec_int* v, SSIZE_T index, VEC_INT_VALUE_T value) {
    assert_notnull(v);
    if (index == v->_size) { // add at the end
        vec_int_push(v, value);
        return;
    }
    assert_valid_index(v, index);
    if (v->_size == v->_cap) {
        vec_int_grow(v);
    }
    for (SSIZE_T i = v->_size - 1; i >= index; --i) {
        v->_values[i + 1] = v->_values[i];
        if (!i) // if i == 0, --i will wrap!
            break;
    }
    v->_values[index] = value;
    v->_size++;
}

void vec_int_add(vec_int* v, VEC_INT_VALUE_T value) {
    assert_notnull(v);
    assert_notnull(value);
    SSIZE_T high = v->_size - 1;
    if (!v->_size || v->_values[high] <= value) {
        vec_int_push(v,
                     value); // vec is empty -or- nonempty and value >= high
    } else {
        SSIZE_T low = 0;
        while (low < high) {
            SSIZE_T mid = (low + high) / 2;
            if (v->_values[mid] > value)
                high = mid;
            else
                low = mid + 1;
        }
        vec_int_insert(v, low, value);
    }
}

VEC_INT_VALUE_T vec_int_replace(vec_int* v, SSIZE_T index,
                                VEC_INT_VALUE_T value) {
    assert_notnull(v);
    assert_valid_index(v, index);
    VEC_INT_VALUE_T old = v->_values[index];
    v->_values[index] = value;
    return old;
}

void vec_int_remove(vec_int* v, SSIZE_T index) {
    assert_notnull(v);
    assert_valid_index(v, index);
    for (SSIZE_T i = index; i < v->_size; ++i) {
        v->_values[i] = v->_values[i + 1];
    }
    v->_size--;
}

VEC_INT_VALUE_T vec_int_take(vec_int* v, SSIZE_T index) {
    assert_notnull(v);
    assert_valid_index(v, index);
    VEC_INT_VALUE_T old = v->_values[index];
    vec_int_remove(v, index);
    return old;
}

VEC_INT_VALUE_T vec_int_pop(vec_int* v) {
    assert_notnull(v);
    assert_nonempty(v);
    return v->_values[--v->_size];
}

void vec_int_push(vec_int* v, VEC_INT_VALUE_T value) {
    assert_notnull(v);
    if (v->_size == v->_cap) {
        vec_int_grow(v);
    }
    v->_values[v->_size++] = value;
}

vec_int vec_int_copy(const vec_int* v) {
    assert_notnull(v);
    vec_int vc = vec_int_alloc_cap(v->_size);
    for (SSIZE_T i = 0; i < v->_size; ++i) {
        vec_int_push(&vc, v->_values[i]);
    }
    return vc;
}

void vec_int_merge(vec_int* v1, vec_int* v2) {
    assert_notnull(v1);
    assert_notnull(v2);
    if ((v1->_cap - v1->_size) < v2->_size) { // v1 doesn't have enough cap
        SSIZE_T cap = v1->_size + v2->_size;
        VEC_INT_VALUE_T* p =
            realloc(v1->_values, cap * sizeof(VEC_INT_VALUE_T));
        assert_alloc(p);
        v1->_values = p;
        v1->_cap = cap;
    }
    for (SSIZE_T i = 0; i < v2->_size; ++i) {
        v1->_values[v1->_size++] = v2->_values[i]; // push
    }
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
    for (SSIZE_T i = 0; i < v1->_size; ++i) {
        if (v1->_values[i] != v2->_values[i])
            return false;
    }
    return true;
}

SSIZE_T vec_int_find(const vec_int* v, VEC_INT_VALUE_T value) {
    assert_notnull(v);
    for (SSIZE_T i = 0; i < v->_size; ++i) {
        if (v->_values[i] == value) {
            return i;
        }
    }
    return VEC_NOT_FOUND;
}

VEC_INT_VALUE_T intcmp(const void* a, const void* b) {
    return (*(VEC_INT_VALUE_T*)a - *(VEC_INT_VALUE_T*)b);
}

void vec_int_sort(vec_int* v) {
    assert_notnull(v);
    if (v->_size) {
        qsort(v->_values, v->_size, sizeof(VEC_INT_VALUE_T), intcmp);
    }
}

SSIZE_T vec_int_search(const vec_int* v, VEC_INT_VALUE_T i) {
    assert_notnull(v);
    if (v->_size) {
        const VEC_INT_VALUE_T* p = bsearch(&i, v->_values, v->_size,
                                           sizeof(VEC_INT_VALUE_T), intcmp);
        if (p) {
            return p - v->_values;
        }
    }
    return VEC_NOT_FOUND;
}

char* vec_int_tostring(const vec_int* v) {
    assert_notnull(v);
    const SSIZE_T BUF_SIZE = 128;
    const SSIZE_T VEC_SIZE = vec_int_size(v);
    SSIZE_T cap = VEC_SIZE * 4;
    char* s = malloc(cap);
    assert_alloc(s);
    SSIZE_T pos = 0;
    char buf[BUF_SIZE];
    for (SSIZE_T i = 0; i < VEC_SIZE; ++i) {
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
    const SSIZE_T BLOCK_SIZE = 1024 * 1024;
    SSIZE_T cap = (v->_cap < BLOCK_SIZE) ? v->_cap * 2 : v->_cap + BLOCK_SIZE;
    VEC_INT_VALUE_T* p = realloc(v->_values, cap * sizeof(VEC_INT_VALUE_T));
    assert_alloc(p);
    v->_values = p;
    v->_cap = cap;
}
