// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec.h"
#include "../sx/sx.h"
#include <assert.h>
#include <stdlib.h>

const void* VEC_ERR_INDEX_OUT_OF_RANGE = "index out of range";
const size_t VEC_BLOCK_SIZE = 4096;

vec vec_alloc_(vec_alloc_args args) {
    assert(args.eq && "must provide an eq function");
    vec v = {.size = 0,
             .cap = args.cap,
             .values = malloc(args.cap * sizeof(void*)),
             .eq = args.eq,
             .destroy = args.destroy};
    return v;
}

inline void vec_free(vec* v) { vec_clear(v); }

void vec_clear(vec* v) {
    if (v->destroy) {
        for (size_t i = 0; i < v->size; i++) {
            v->destroy(v->values[i]);
        }
    }
    free(v->values);
    v->values = NULL;
    v->size = 0;
    v->cap = 0;
}

const void* vec_get(vec* v, size_t index) {
    assert(index < v->size && VEC_ERR_INDEX_OUT_OF_RANGE);
    return v->values[index];
}

void vec_set(vec* v, size_t index, void* value) {
    assert(index < v->size && VEC_ERR_INDEX_OUT_OF_RANGE);
    if (v->destroy) {
        v->destroy(v->values[index]);
    }
    v->values[index] = value;
}

void* vec_replace(vec* v, size_t index, void* value) {
    assert(index < v->size && VEC_ERR_INDEX_OUT_OF_RANGE);
    void* old = v->values[index];
    v->values[index] = value;
    return old;
}

void vec_remove(vec* v, size_t index) {
    assert(index < v->size && VEC_ERR_INDEX_OUT_OF_RANGE);
    void* old = v->values[index];
    for (size_t i = index; i < v->size; i++) {
        v->values[i] = v->values[i + 1];
    }
    v->values[v->size - 1] = NULL;
    v->size--;
    if (v->destroy) {
        v->destroy(old);
    }
}

void* vec_take(vec* v, size_t index) {
    assert(index < v->size && VEC_ERR_INDEX_OUT_OF_RANGE);
    void* old = v->values[index];
    for (size_t i = index; i < v->size; i++) {
        v->values[i] = v->values[i + 1];
    }
    v->values[v->size - 1] = NULL;
    v->size--;
    return old;
}

void* vec_pop(vec* v) {
    assert(v->size > 0 && "can't pop empty vec");
    return v->values[--v->size];
}

void vec_push(vec* v, void* value) {
    if (v->size == v->cap) {
        size_t cap = (v->cap < VEC_BLOCK_SIZE) ? v->cap * 2
                                               : v->cap + VEC_BLOCK_SIZE;
        v->values = realloc(v->values, cap);
        v->cap = cap;
    }
    v->values[v->size++] = value;
}

vec_find_result vec_find(vec* v, void* value) {
    for (size_t i = 0; i < v->size; i++) {
        if (v->eq(v->values[i], value)) {
            return (vec_find_result){.index = i, .found = true};
        }
    }
    return (vec_find_result){.index = 0, .found = false};
}

vec vec_copy(vec* v, void* (*cp)(void*)) {
    vec vc = vec_alloc(.cap = v->size, .eq = v->eq, .destroy = v->destroy);
    for (size_t i = 0; i < v->size; i++) {
        vec_push(&vc, cp(v->values[i]));
    }
    return vc;
}

bool vec_eq(vec* v1, vec* v2) {
    if (v1->eq != v2->eq)
        return false;
    if (v1->destroy != v2->destroy)
        return false;
    if (v1->size != v2->size)
        return false;
    for (size_t i = 0; i < v1->size; i++) {
        if (!v1->eq(v1->values[i], v2->values[i]))
            return false;
    }
    return true;
}
