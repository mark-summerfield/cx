// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_int.h"
#include <assert.h>
#include <stdlib.h>

#define assert_valid_index(v, index)                                       \
    assert((index) < (v)->_size && "index out of range")

#define assert_alloc(p) assert((p) && "failed to acquire memory")

vec_int vec_int_alloc(size_t capacity) {
    capacity = capacity ? capacity : 32;
    int* values = malloc(capacity * sizeof(int));
    assert_alloc(values);
    vec_int v = {._size = 0, ._cap = capacity, ._values = values};
    return v;
}

inline void vec_int_free(vec_int* v) { vec_int_clear(v); }

void vec_int_clear(vec_int* v) {
    free(v->_values);
    v->_values = NULL;
    v->_size = 0;
    v->_cap = 0;
}

int vec_int_get(vec_int* v, size_t index) {
    assert_valid_index(v, index);
    return v->_values[index];
}

void vec_int_set(vec_int* v, size_t index, int value) {
    assert_valid_index(v, index);
    v->_values[index] = value;
}

int vec_int_replace(vec_int* v, size_t index, int value) {
    assert_valid_index(v, index);
    int old = v->_values[index];
    v->_values[index] = value;
    return old;
}

void vec_int_remove(vec_int* v, size_t index) {
    assert_valid_index(v, index);
    for (size_t i = index; i < v->_size; i++) {
        v->_values[i] = v->_values[i + 1];
    }
    v->_size--;
}

int vec_int_take(vec_int* v, size_t index) {
    assert_valid_index(v, index);
    int old = v->_values[index];
    for (size_t i = index; i < v->_size; i++) {
        v->_values[i] = v->_values[i + 1];
    }
    v->_size--;
    return old;
}

int vec_int_pop(vec_int* v) {
    assert(v->_size > 0 && "can't pop empty vec_int");
    return v->_values[--v->_size];
}

void vec_int_push(vec_int* v, int value) {
    const size_t BLOCK_SIZE = 1024 * 1024;
    if (v->_size == v->_cap) {
        size_t cap =
            (v->_cap < BLOCK_SIZE) ? v->_cap * 2 : v->_cap + BLOCK_SIZE;
        int* values = realloc(v->_values, cap * sizeof(int));
        assert_alloc(values);
        v->_values = values;
        v->_cap = cap;
    }
    v->_values[v->_size++] = value;
}

vec_int_find_result vec_int_find(vec_int* v, int value) {
    for (size_t i = 0; i < v->_size; i++) {
        if (v->_values[i] == value) {
            return (vec_int_find_result){.index = i, .found = true};
        }
    }
    return (vec_int_find_result){.index = 0, .found = false};
}

vec_int vec_int_copy(vec_int* v) {
    vec_int vc = vec_int_alloc(v->_size);
    for (size_t i = 0; i < v->_size; i++) {
        vec_int_push(&vc, v->_values[i]);
    }
    return vc;
}

bool vec_int_eq(vec_int* v1, vec_int* v2) {
    if (v1->_size != v2->_size)
        return false;
    for (size_t i = 0; i < v1->_size; i++) {
        if (v1->_values[i] != v2->_values[i])
            return false;
    }
    return true;
}
