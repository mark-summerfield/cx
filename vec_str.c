// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

bool vec_str_eq(void* s, void* t) { return strcmp((s), (t)) == 0; }

void vec_str_destroy(void* value) { free((char*)value); }

void* vec_str_cp(void* value) { return (void*)strdup((char*)value); }

inline const char* vec_str_get_last(vec* v) {
    return (char*)v->_values[v->_size - 1];
}

#define assert_size_ok(expr) assert((expr) <= 0 && "buffer too small")

char* vec_str_dump(vec* v, const char* name) {
    const size_t SIZE = 500;
    char* buffer = malloc(SIZE);
    char* out = buffer;
    long long size = SIZE - 1;
    size_t n;
    if (name) {
        n = snprintf(out, size, "%s=", name);
        out += n;
        size -= n;
        assert_size_ok(size);
    }
    n = snprintf(out, size, "[");
    out += n;
    size -= n;
    size -= n;
    assert_size_ok(size);
    for (size_t i = 0; i < v->_size; i++) {
        n = snprintf(out, size, "\"%s\"", (char*)v->_values[i]);
        out += n;
        size -= n;
        size -= n;
        assert_size_ok(size);
        if (i + 1 < v->_size) {
            n = snprintf(out, size, " ");
            out += n;
            size -= n;
            size -= n;
            assert_size_ok(size);
        }
    }
    size -= n;
    assert_size_ok(size - 2);
    snprintf(out, size, "]");
    return buffer;
}
