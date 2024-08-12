// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str.h"
#include "cx.h"
#include <stdlib.h>
#include <string.h>

bool vec_str_eq(void* s, void* t) { return strcmp((s), (t)) == 0; }

void vec_str_destroy(void* value) { free((char*)value); }

void* vec_str_cp(void* value) { return strdup((char*)value); }

vec vec_str_alloc_split(const char* s, const char* sep) {
    assert(s && "can't split empty string");
    assert(sep && "can't split with empty sep");
    size_t sep_size = strlen(sep);
    vec v = vec_str_alloc();
    const char* p = s;
    while (p) {
        const char* q = strstr(p, sep);
        if (q) {
            size_t n = q - p;
            vec_str_push(&v, strndup(p, n));
            p = q + sep_size;
        } else {
            if (strlen(p)) {
                vec_str_push(&v, strdup(p));
            }
            break;
        }
    }
    return v;
}

inline const char* vec_str_get_last(const vec* v) {
    return (char*)v->_values[v->_size - 1];
}

char* vec_str_join(const vec* v, const char* sep) {
    const size_t VEC_SIZE = vec_size(v);
    const size_t SEP_SIZE = sep ? strlen(sep) : 0;
    size_t total_size = 0;
    size_t sizes[VEC_SIZE];
    for (size_t i = 0; i < VEC_SIZE; ++i) {
        size_t size = strlen(vec_str_get(v, i));
        sizes[i] = size;
        total_size += size + SEP_SIZE;
    }
    total_size -= SEP_SIZE; // don't want one at the end
    total_size++;           // allow for \0
    char* s = malloc(total_size);
    assert_alloc(s);
    char* p = s;
    for (size_t i = 0; i < VEC_SIZE; ++i) {
        size_t size = sizes[i];
        strncpy(p, strndup(vec_str_get(v, i), size), size);
        p += size;
        if (sep && (i + 1 < VEC_SIZE)) { // avoid adding one at the end
            strncpy(p, strndup(sep, SEP_SIZE), SEP_SIZE);
            p += SEP_SIZE;
        }
    }
    return s;
}

void vec_str_sort(vec* v, cmpfn cmp) {
    printf("TODO vec_str_sort"); // TODO
}

vec_found_index vec_str_search(const vec* v, const char* s, cmpfn cmp) {
    vec_found_index result = {0, false};
    printf("TODO vec_str_bsearch"); // TODO
    return result;
}
