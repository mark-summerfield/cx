// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str.h"
#include "../sx/sx.h"
#include <assert.h>
#include <stdlib.h>

const char* VEC_STR_ERR_INDEX_OUT_OF_RANGE = "index out of range";
const size_t VEC_STR_BLOCK_SIZE = 4096;

vec_str vec_str_alloc_(vec_str_alloc_args args) {
    vec_str v = {.size = 0,
                 .cap = args.cap,
                 .own_values = args.own_values,
                 .values = malloc(args.cap * sizeof(char*))};
    return v;
}

inline void vec_str_free(vec_str* v) { vec_str_clear(v); }

void vec_str_clear(vec_str* v) {
    if (v->own_values) {
        for (size_t i = 0; i < v->size; i++) {
            free(v->values[i]);
        }
    }
    free(v->values);
    v->values = NULL;
    v->size = 0;
    v->cap = 0;
}

const char* vec_str_get(vec_str* v, size_t index) {
    assert(index < v->size && VEC_STR_ERR_INDEX_OUT_OF_RANGE);
    return v->values[index];
}

void vec_str_set(vec_str* v, size_t index, char* value) {
    assert(index < v->size && VEC_STR_ERR_INDEX_OUT_OF_RANGE);
    if (v->own_values) {
        free(v->values[index]);
    }
    v->values[index] = value;
}

char* vec_str_replace(vec_str* v, size_t index, char* value) {
    assert(index < v->size && VEC_STR_ERR_INDEX_OUT_OF_RANGE);
    char* old = v->values[index];
    v->values[index] = value;
    return old;
}

void vec_str_remove(vec_str* v, size_t index) {
    assert(index < v->size && VEC_STR_ERR_INDEX_OUT_OF_RANGE);
    char* old = v->values[index];
    for (size_t i = index; i < v->size; i++) {
        v->values[i] = v->values[i + 1];
    }
    v->values[v->size - 1] = NULL;
    v->size--;
    if (v->own_values) {
        free(old);
    }
}

char* vec_str_take(vec_str* v, size_t index) {
    assert(index < v->size && VEC_STR_ERR_INDEX_OUT_OF_RANGE);
    char* old = v->values[index];
    for (size_t i = index; i < v->size; i++) {
        v->values[i] = v->values[i + 1];
    }
    v->values[v->size - 1] = NULL;
    v->size--;
    return old;
}

const char* vec_str_pop(vec_str* v) {
    assert(v->size > 0 && "can't pop empty vec");
    return v->values[--v->size];
}

void vec_str_push(vec_str* v, char* value) {
    if (v->size == v->cap) {
        size_t cap = (v->cap < VEC_STR_BLOCK_SIZE)
                         ? v->cap * 2
                         : v->cap + VEC_STR_BLOCK_SIZE;
        v->values = realloc(v->values, cap);
        v->cap = cap;
    }
    v->values[v->size++] = value;
}

vec_str_find_result vec_str_find(vec_str* v, char* value) {
    for (size_t i = 0; i < v->size; i++) {
        if (sx_eq(v->values[i], value)) {
            return (vec_str_find_result){.index = i, .found = true};
        }
    }
    return (vec_str_find_result){.index = 0, .found = false};
}

vec_str vec_str_copy(vec_str* v) {
    vec_str vc = vec_str_alloc(.cap = v->size, .own_values = true);
    for (size_t i = 0; i < v->size; i++) {
        vec_str_push(&vc, strdup(v->values[i]));
    }
    return vc;
}
