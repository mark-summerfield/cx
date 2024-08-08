// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str.h"
#include <stdlib.h>
#include <string.h>

bool vec_str_eq(void* s, void* t) { return strcmp((s), (t)) == 0; }

void vec_str_destroy(void* value) { free((char*)value); }

void* vec_str_cp(void* value) { return (void*)strdup((char*)value); }

void vec_str_dump(vec* v, const char* name) {
    const size_t SIZE = 500;
    char* buffer = malloc(SIZE);
    char* out = buffer;
    size_t size = SIZE - 1;
    size_t n;
    if (name) {
        n = snprintf(out, size, "%s=", name);
        out += n;
        size -= n;
    }
    n = snprintf(out, size, "[");
    out += n;
    size -= n;
    for (size_t i = 0; i < v->_size; i++) {
        n = snprintf(out, size, "\"%s\"", (char*)v->_values[i]);
        out += n;
        size -= n;
        if (i + 1 < v->_size) {
            n = snprintf(out, size, " ");
            out += n;
            size -= n;
        }
    }
    snprintf(out, size, "]\n");
    return buffer;
}
