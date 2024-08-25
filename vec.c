// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec.h"
#include <stdlib.h>

static void vec_grow(Vec* vec);

// This *is* used, via a macro in vec.h; clang-format gets confused.
Vec vec_alloc_(VecAllocArgs args) {
    assert(args.cmp && "must provide a cmp function");
    assert(((args.cpy && args.destroy) || (!args.cpy && !args.destroy)) &&
           "must provide both cpy and destroy (owns) or neither");
    args.cap = args.cap > 0 ? args.cap : VEC_INITIAL_CAP;
    void** values = malloc(args.cap * sizeof(void*));
    assert_alloc(values);
    return (Vec){._size = 0,
                 ._cap = args.cap,
                 ._values = values,
                 ._cmp = args.cmp,
                 ._cpy = args.cpy,
                 ._destroy = args.destroy};
}

void vec_free(Vec* vec) {
    assert_notnull(vec);
    vec_clear(vec);
    free(vec->_values);
    vec->_values = NULL;
    vec->_cap = 0;
    vec->_cpy = NULL;
    vec->_destroy = NULL;
}

void vec_clear(Vec* vec) {
    assert_notnull(vec);
    if (vec->_destroy)
        for (int i = 0; i < vec->_size; ++i)
            vec->_destroy(vec->_values[i]);
    vec->_size = 0;
}

void* vec_get(const Vec* vec, int index) {
    assert_notnull(vec);
    assert_valid_index(vec, index);
    return vec->_values[index];
}

inline void* vec_get_last(const Vec* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->_values[vec->_size - 1];
}

void vec_set(Vec* vec, int index, void* value) {
    assert_notnull(vec);
    assert_notnull(value);
    assert_valid_index(vec, index);
    if (vec->_destroy)
        vec->_destroy(vec->_values[index]);
    vec->_values[index] = value;
}

void vec_insert(Vec* vec, int index, void* value) {
    assert_notnull(vec);
    assert_notnull(value);
    assert_valid_index(vec, index);
    if (vec->_size == vec->_cap)
        vec_grow(vec);
    for (int i = vec->_size; i > index; --i)
        vec->_values[i] = vec->_values[i - 1];
    vec->_values[index] = value;
    vec->_size++;
}

void vec_add(Vec* vec, void* value) {
    assert_notnull(vec);
    assert_notnull(value);
    int high = vec->_size - 1;
    if (!vec->_size || vec->_cmp(&vec->_values[high], &value) <= 0) {
        vec_push(vec,
                 value); // Vec is empty -or- nonempty and value >= high
    } else {
        int low = 0;
        while (low < high) {
            int mid = (low + high) / 2;
            if (vec->_cmp(&vec->_values[mid], &value) > 0)
                high = mid;
            else
                low = mid + 1;
        }
        vec_insert(vec, low, value);
    }
}

void* vec_replace(Vec* vec, int index, void* value) {
    assert_notnull(vec);
    assert_notnull(value);
    assert_valid_index(vec, index);
    void* old = vec->_values[index];
    vec->_values[index] = value;
    return old;
}

inline void vec_remove(Vec* vec, int index) {
    assert_notnull(vec);
    void* value = vec_take(vec, index); // vec_take checks index
    if (vec->_destroy)
        vec->_destroy(value);
}

void* vec_take(Vec* vec, int index) {
    assert_notnull(vec);
    assert_valid_index(vec, index);
    void* old = vec->_values[index];
    for (int i = index; i < vec->_size; ++i)
        vec->_values[i] = vec->_values[i + 1];
    vec->_size--;
    vec->_values[vec->_size] = NULL;
    return old;
}

void* vec_pop(Vec* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->_values[--vec->_size];
}

void vec_push(Vec* vec, void* value) {
    assert_notnull(vec);
    assert_notnull(value);
    if (vec->_size == vec->_cap)
        vec_grow(vec);
    vec->_values[vec->_size++] = value;
}

Vec vec_copy(const Vec* vec, bool owns) {
    assert_notnull(vec);
    assert((!owns || (owns && vec->_destroy && vec->_cpy)) &&
           "can't copy borrowed to owned");
#pragma GCC diagnostic ignored "-Woverride-init"
#pragma GCC diagnostic push
    Vec out = vec_alloc(.cap = vec->_size, .cmp = vec->_cmp,
                        .cpy = owns ? vec->_cpy : NULL,
                        .destroy = owns ? vec->_destroy : NULL);
#pragma GCC diagnostic pop
    for (int i = 0; i < vec->_size; ++i) {
        void* value = vec->_values[i];
        vec_push(&out, owns ? vec->_cpy(value) : value);
    }
    return out;
}

void vec_merge(Vec* vec1, Vec* vec2) {
    assert_notnull(vec1);
    assert_notnull(vec2);
    assert(vec1->_cmp == vec2->_cmp && vec1->_cpy == vec2->_cpy &&
           vec1->_destroy == vec2->_destroy && "incompatible vecs");
    if ((vec1->_cap - vec1->_size) <
        vec2->_size) { // vec1 doesn't have enough cap
        int cap = vec1->_size + vec2->_size;
        void** p = realloc(vec1->_values, cap * sizeof(void*));
        assert_alloc(p);
        vec1->_values = p;
        vec1->_cap = cap;
    }
    for (int i = 0; i < vec2->_size; ++i)
        vec1->_values[vec1->_size++] = vec2->_values[i]; // push
    free(vec2->_values);
    vec2->_values = NULL;
    vec2->_cap = 0;
    vec2->_size = 0;
    vec2->_cpy = NULL;
    vec2->_destroy = NULL;
}

bool vec_equal(const Vec* vec1, const Vec* vec2) {
    assert_notnull(vec1);
    assert_notnull(vec2);
    if (vec1->_size != vec2->_size)
        return false;
    for (int i = 0; i < vec1->_size; ++i)
        if (vec1->_cmp(&vec1->_values[i], &vec2->_values[i]))
            return false;
    return true;
}

bool vec_same(const Vec* vec1, const Vec* vec2) {
    assert_notnull(vec1);
    assert_notnull(vec2);
    if (vec1->_cmp != vec2->_cmp || vec1->_cpy != vec2->_cpy ||
        vec1->_destroy != vec2->_destroy)
        return false;
    return vec_equal(vec1, vec2);
}

int vec_find(const Vec* vec, const void* value) {
    assert_notnull(vec);
    assert_notnull(value);
    for (int i = 0; i < vec->_size; ++i)
        if (vec->_cmp(&vec->_values[i], &value) == 0)
            return i;
    return VEC_NOT_FOUND;
}

int vec_find_last(const Vec* vec, const void* value) {
    assert_notnull(vec);
    assert_notnull(value);
    for (int i = vec->_size - 1; i >= 0; --i)
        if (vec->_cmp(&vec->_values[i], &value) == 0)
            return i;
    return VEC_NOT_FOUND;
}

void vec_sort(Vec* vec) {
    assert_notnull(vec);
    if (vec->_size)
        qsort(vec->_values, vec->_size, sizeof(void*), vec->_cmp);
}

int vec_search(const Vec* vec, const void* value) {
    assert_notnull(vec);
    assert_notnull(value);
    if (vec->_size) {
        void** p = bsearch(&value, vec->_values, vec->_size, sizeof(void*),
                           vec->_cmp);
        if (p)
            return p - vec->_values;
    }
    return VEC_NOT_FOUND;
}

static void vec_grow(Vec* vec) {
    const int BLOCK_SIZE = 1024 * 1024;
    int cap =
        (vec->_cap < BLOCK_SIZE) ? vec->_cap * 2 : vec->_cap + BLOCK_SIZE;
    void** p = realloc(vec->_values, cap * sizeof(void*));
    assert_alloc(p);
    vec->_values = p;
    vec->_cap = cap;
}
