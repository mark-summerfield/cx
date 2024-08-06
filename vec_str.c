// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str.h"
#include <assert.h>
#include <stdlib.h>

const char* ERR_INDEX_OUT_OF_RANGE = "index out of range";

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
        for (int i = 0; i < v->size; i++) {
            free(v->values[i]);
        }
    }
    free(v->values);
    v->values = NULL;
    v->size = 0;
    v->cap = 0;
}

char* vec_str_get(vec_str* v, size_t index) {
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

char* vec_str_exch(vec_str* v, size_t index, char* value) {
    assert(index < v->size && ERR_INDEX_OUT_OF_RANGE);
    char* old = v->values[index];
    v->values[index] = value;
    return old;
}
