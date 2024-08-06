// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec.h"
#include <stdlib.h>

vec vec_alloc_(vec_alloc_args args) {
    vec v = {.size = 0,
             .cap = args.cap,
             .own_values = args.own_values,
             .values = malloc(args.cap * sizeof(void*))};
    return v;
}

void vec_free(vec* v) { vec_clear(v); }

void vec_clear(vec* v) {
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
