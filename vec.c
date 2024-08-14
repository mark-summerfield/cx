// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec.h"
#include <stdlib.h>

void _vec_grow(vec* v);

vec vec_alloc_(vec_alloc_args args) {
    assert(args.cmp && "must provide a cmp function");
    assert(args.cpy && "must provide a cpy function");
    assert(args.destroy && "must provide a destroy function");
    void** values = malloc(args.cap * sizeof(void*));
    assert_alloc(values);
    return (vec){._size = 0,
                 ._cap = args.cap,
                 ._values = values,
                 ._cmp = args.cmp,
                 ._cpy = args.cpy,
                 ._destroy = args.destroy};
}

void vec_free(vec* v) {
    vec_clear(v);
    free(v->_values);
    v->_values = NULL;
    v->_cap = 0;
}

void vec_clear(vec* v) {
    for (size_t i = 0; i < v->_size; ++i) {
        v->_destroy(v->_values[i]);
    }
    v->_size = 0;
}

const void* vec_get(const vec* v, size_t index) {
    assert_valid_index(v, index);
    return v->_values[index];
}

inline const void* vec_get_last(const vec* v) {
    return v->_values[v->_size - 1];
}

void vec_set(vec* v, size_t index, void* value) {
    assert_valid_index(v, index);
    v->_destroy(v->_values[index]);
    v->_values[index] = value;
}

void vec_insert(vec* v, size_t index, void* value) {
    if (index == v->_size) { // add at the end
        vec_push(v, value);
        return;
    }
    assert_valid_index(v, index);
    if (v->_size == v->_cap) {
        _vec_grow(v);
    }
    for (size_t i = v->_size - 1; i >= index; --i) {
        v->_values[i + 1] = v->_values[i];
        if (!i) // if i == 0, --i will wrap!
            break;
    }
    v->_values[index] = value;
    v->_size++;
}

void* vec_replace(vec* v, size_t index, void* value) {
    assert_valid_index(v, index);
    void* old = v->_values[index];
    v->_values[index] = value;
    return old;
}

inline void vec_remove(vec* v, size_t index) {
    v->_destroy(vec_take(v, index));
}

void* vec_take(vec* v, size_t index) {
    assert_valid_index(v, index);
    void* old = v->_values[index];
    for (size_t i = index; i < v->_size; ++i) {
        v->_values[i] = v->_values[i + 1];
    }
    v->_size--;
    v->_values[v->_size] = NULL;
    return old;
}

void* vec_pop(vec* v) {
    assert(v->_size > 0 && "can't pop empty vec");
    return v->_values[--v->_size];
}

void vec_push(vec* v, void* value) {
    if (v->_size == v->_cap) {
        _vec_grow(v);
    }
    v->_values[v->_size++] = value;
}

vec vec_copy(const vec* v) {
#pragma GCC diagnostic ignored "-Woverride-init"
#pragma GCC diagnostic push
    vec vc =
        vec_alloc(.cap = v->_size ? v->_size : VEC_INITIAL_CAP,
                  .cmp = v->_cmp, .cpy = v->_cpy, .destroy = v->_destroy);
#pragma GCC diagnostic pop
    for (size_t i = 0; i < v->_size; ++i) {
        vec_push(&vc, v->_cpy(v->_values[i]));
    }
    return vc;
}

void vec_merge(vec* v1, vec* v2) {
    assert(v1->_cmp == v2->_cmp && v1->_cpy == v2->_cpy &&
           v1->_destroy == v2->_destroy && "non-matching vecs");
    if ((v1->_cap - v1->_size) < v2->_size) { // v1 doesn't have enough cap
        size_t cap = v1->_size + v2->_size;
        void** p = realloc(v1->_values, cap * sizeof(void*));
        assert_alloc(p);
        v1->_values = p;
        v1->_cap = cap;
    }
    for (size_t i = 0; i < v2->_size; ++i) {
        v1->_values[v1->_size++] = v2->_values[i]; // push
    }
    free(v2->_values);
    v2->_values = NULL;
    v2->_cap = 0;
    v2->_size = 0;
}

bool vec_equal(const vec* v1, const vec* v2) {
    if (v1->_cmp != v2->_cmp || v1->_cpy != v2->_cpy ||
        v1->_destroy != v2->_destroy || v1->_size != v2->_size)
        return false;
    for (size_t i = 0; i < v1->_size; ++i) {
        if (v1->_cmp(v1->_values[i], v2->_values[i]))
            return false;
    }
    return true;
}

vec_found_index vec_find(const vec* v, const void* value) {
    vec_found_index found_index = {0, false};
    for (size_t i = 0; i < v->_size; ++i) {
        if (v->_cmp(v->_values[i], value) == 0) {
            found_index.index = i;
            found_index.found = true;
            break;
        }
    }
    return found_index;
}

void vec_sort(vec* v) {
    if (v->_size) {
        qsort(v->_values, v->_size, sizeof(v->_values[0]), v->_cmp);
    }
}

vec_found_index vec_search(const vec* v, const void* s) {
    vec_found_index found_index = {0, false};
    if (v->_size) {
        const void* p = bsearch(s, v->_values, v->_size,
                                sizeof(v->_values[0]), v->_cmp);
        if (p) {
#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma GCC diagnostic push
            found_index.index = p - v->_values[0];
#pragma GCC diagnostic pop
            found_index.found = true;
        }
    }
    return found_index;
}

void _vec_grow(vec* v) {
    const size_t BLOCK_SIZE = 1024 * 1024;
    size_t cap =
        (v->_cap < BLOCK_SIZE) ? v->_cap * 2 : v->_cap + BLOCK_SIZE;
    void** p = realloc(v->_values, cap * sizeof(void*));
    assert_alloc(p);
    v->_values = p;
    v->_cap = cap;
}
