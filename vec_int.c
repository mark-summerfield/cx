// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_int.h"
#include <assert.h>
#include <stdlib.h>

#define assert_valid_index(v, index)                                       \
    assert((index) < (v)->size && "index out of range")

vec_int vec_int_alloc(size_t capacity) {
    vec_int v = {.size = 0,
                 .cap = capacity,
                 .values = malloc(capacity * sizeof(int))};
    return v;
}

inline void vec_int_free(vec_int* v) { vec_int_clear(v); }

void vec_int_clear(vec_int* v) {
    free(v->values);
    v->values = NULL;
    v->size = 0;
    v->cap = 0;
}

int vec_int_get(vec_int* v, size_t index) {
    assert_valid_index(v, index);
    return v->values[index];
}

void vec_int_set(vec_int* v, size_t index, int value) {
    assert_valid_index(v, index);
    v->values[index] = value;
}

int vec_int_replace(vec_int* v, size_t index, int value) {
    assert_valid_index(v, index);
    int old = v->values[index];
    v->values[index] = value;
    return old;
}

void vec_int_remove(vec_int* v, size_t index) {
    assert_valid_index(v, index);
    for (size_t i = index; i < v->size; i++) {
        v->values[i] = v->values[i + 1];
    }
    v->size--;
}

int vec_int_take(vec_int* v, size_t index) {
    assert_valid_index(v, index);
    int old = v->values[index];
    for (size_t i = index; i < v->size; i++) {
        v->values[i] = v->values[i + 1];
    }
    v->size--;
    return old;
}

int vec_int_pop(vec_int* v) {
    assert(v->size > 0 && "can't pop empty vec_int");
    return v->values[--v->size];
}

void vec_int_push(vec_int* v, int value) {
    const size_t BLOCK_SIZE = 4096;
    if (v->size == v->cap) {
        size_t cap =
            (v->cap < BLOCK_SIZE) ? v->cap * 2 : v->cap + BLOCK_SIZE;
        v->values = realloc(v->values, cap);
        v->cap = cap;
    }
    v->values[v->size++] = value;
}

vec_int_find_result vec_int_find(vec_int* v, int value) {
    for (size_t i = 0; i < v->size; i++) {
        if (v->values[i] == value) {
            return (vec_int_find_result){.index = i, .found = true};
        }
    }
    return (vec_int_find_result){.index = 0, .found = false};
}

vec_int vec_int_copy(vec_int* v) {
    vec_int vc = vec_int_alloc(v->size);
    for (size_t i = 0; i < v->size; i++) {
        vec_int_push(&vc, v->values[i]);
    }
    return vc;
}

bool vec_int_eq(vec_int* v1, vec_int* v2) {
    if (v1->size != v2->size)
        return false;
    for (size_t i = 0; i < v1->size; i++) {
        if (v1->values[i] != v2->values[i])
            return false;
    }
    return true;
}
