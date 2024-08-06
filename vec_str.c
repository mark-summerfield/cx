// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str.h"
#include "../sx/sx.h"
#include <assert.h>
#include <stdlib.h>

const char* ERR_INDEX_OUT_OF_RANGE = "index out of range";
const size_t BLOCK_SIZE = 4096;

vec_str vec_str_alloc_(vec_str_alloc_args args) {
    vec_str v = {.size = 0,
                 .cap = args.cap,
                 .own_values = args.own_values,
                 .values = malloc(args.cap * sizeof(char*))};
    return v;
}

void vec_str_free(vec_str* v) { vec_str_clear(v); }

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
    assert(index < v->size && ERR_INDEX_OUT_OF_RANGE);
    return v->values[index];
}

void vec_str_set(vec_str* v, size_t index, char* value) {
    assert(index < v->size && ERR_INDEX_OUT_OF_RANGE);
    if (v->own_values) {
        free(v->values[index]);
    }
    v->values[index] = value;
}

const char* vec_str_exch(vec_str* v, size_t index, char* value) {
    assert(index < v->size && ERR_INDEX_OUT_OF_RANGE);
    const char* old = v->values[index];
    v->values[index] = value;
    return old;
}

void vec_str_remove3(vec_str* v, size_t begin, size_t end) {
    assert(begin < v->size && end < v->size && ERR_INDEX_OUT_OF_RANGE);
    for (size_t i = begin; i < end; i++) {
        if (v->own_values) {
            free(v->values[i]);
        }
        v->values[i] = NULL;
    }
    v->size -= (end - begin);
}

const char* vec_str_pop(vec_str* v) {
    assert(v->size > 0 && "can't pop empty vec");
    return v->values[--v->size];
}

void vec_str_push(vec_str* v, char* value) {
    if (v->size == v->cap) {
        size_t cap =
            (v->cap < BLOCK_SIZE) ? v->cap * 2 : v->cap + BLOCK_SIZE;
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

vec_str copy(vec_str* v) {
    vec_str vc = vec_str_alloc(.cap = v->size, .own_values = true);
    for (size_t i = 0; i < v->size; i++) {
        vec_str_push(&vc, strdup(v->values[i]));
    }
    return vc;
}
