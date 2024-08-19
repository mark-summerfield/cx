// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str.h"
#include "sx.h"
#include <stdlib.h>
#include <string.h>

static void vec_str_grow(vec_str* v);

vec_str vec_str_alloc_cap(ptrdiff_t cap) {
    char** values = malloc(cap * sizeof(char*));
    assert_alloc(values);
    return (vec_str){._count = 0, ._cap = cap, ._values = values};
}

void vec_str_free(vec_str* v) {
    assert_notnull(v);
    vec_str_clear(v);
    free(v->_values);
    v->_values = NULL;
    v->_cap = 0;
}

void vec_str_clear(vec_str* v) {
    assert_notnull(v);
    for (ptrdiff_t i = 0; i < v->_count; ++i) {
        free(v->_values[i]);
    }
    v->_count = 0;
}

const char* vec_str_get(const vec_str* v, ptrdiff_t index) {
    assert_notnull(v);
    assert_valid_index(v, index);
    return v->_values[index];
}

inline const char* vec_str_get_last(const vec_str* v) {
    assert_notnull(v);
    assert_nonempty(v);
    return v->_values[v->_count - 1];
}

void vec_str_set(vec_str* v, ptrdiff_t index, char* value) {
    assert_notnull(v);
    assert_notnull(value);
    assert_valid_index(v, index);
    free(v->_values[index]);
    v->_values[index] = value;
}

void vec_str_insert(vec_str* v, ptrdiff_t index, char* value) {
    assert_notnull(v);
    assert_notnull(value);
    if (index == v->_count) { // add at the end
        vec_str_push(v, value);
        return;
    }
    assert_valid_index(v, index);
    if (v->_count == v->_cap) {
        vec_str_grow(v);
    }
    for (ptrdiff_t i = v->_count - 1; i >= index; --i) {
        v->_values[i + 1] = v->_values[i];
        if (!i) // if i == 0, --i will wrap!
            break;
    }
    v->_values[index] = value;
    v->_count++;
}

void vec_str_add(vec_str* v, char* value) {
    assert_notnull(v);
    assert_notnull(value);
    ptrdiff_t high = v->_count - 1;
    if (!v->_count || strcmp(v->_values[high], value) <= 0) {
        vec_str_push(v,
                     value); // vec is empty -or- nonempty and value >= high
    } else {
        ptrdiff_t low = 0;
        while (low < high) {
            ptrdiff_t mid = (low + high) / 2;
            if (strcmp(v->_values[mid], value) > 0)
                high = mid;
            else
                low = mid + 1;
        }
        vec_str_insert(v, low, value);
    }
}

char* vec_str_replace(vec_str* v, ptrdiff_t index, char* value) {
    assert_notnull(v);
    assert_notnull(value);
    assert_valid_index(v, index);
    char* old = v->_values[index];
    v->_values[index] = value;
    return old;
}

inline void vec_str_remove(vec_str* v, ptrdiff_t index) {
    assert_notnull(v);
    free(vec_str_take(v, index));
}

char* vec_str_take(vec_str* v, ptrdiff_t index) {
    assert_notnull(v);
    assert_valid_index(v, index);
    char* old = v->_values[index];
    for (ptrdiff_t i = index; i < v->_count; ++i) {
        v->_values[i] = v->_values[i + 1];
    }
    v->_count--;
    v->_values[v->_count] = NULL;
    return old;
}

char* vec_str_pop(vec_str* v) {
    assert_notnull(v);
    assert_nonempty(v);
    return v->_values[--v->_count];
}

void vec_str_push(vec_str* v, char* value) {
    assert_notnull(v);
    assert_notnull(value);
    if (v->_count == v->_cap) {
        vec_str_grow(v);
    }
    v->_values[v->_count++] = value;
}

vec_str vec_str_copy(const vec_str* v) {
    assert_notnull(v);
    vec_str vc = vec_str_alloc_cap(v->_count ? v->_count : VEC_INITIAL_CAP);
    for (ptrdiff_t i = 0; i < v->_count; ++i) {
        vec_str_push(&vc, strdup(v->_values[i]));
    }
    return vc;
}

void vec_str_merge(vec_str* v1, vec_str* v2) {
    assert_notnull(v1);
    assert_notnull(v2);
    if ((v1->_cap - v1->_count) <
        v2->_count) { // v1 doesn't have enough cap
        ptrdiff_t cap = v1->_count + v2->_count;
        char** p = realloc(v1->_values, cap * sizeof(char*));
        assert_alloc(p);
        v1->_values = p;
        v1->_cap = cap;
    }
    for (ptrdiff_t i = 0; i < v2->_count; ++i) {
        v1->_values[v1->_count++] = v2->_values[i]; // push
    }
    free(v2->_values);
    v2->_values = NULL;
    v2->_cap = 0;
    v2->_count = 0;
}

bool vec_str_equal(const vec_str* v1, const vec_str* v2) {
    assert_notnull(v1);
    assert_notnull(v2);
    for (ptrdiff_t i = 0; i < v1->_count; ++i) {
        if (strcmp(v1->_values[i], v2->_values[i]))
            return false;
    }
    return true;
}

ptrdiff_t vec_str_find(const vec_str* v, const char* value) {
    assert_notnull(v);
    assert_notnull(value);
    for (ptrdiff_t i = 0; i < v->_count; ++i) {
        if (strcmp(v->_values[i], value) == 0) {
            return i;
        }
    }
    return VEC_NOT_FOUND;
}

void vec_str_sort(vec_str* v) {
    assert_notnull(v);
    qsort(v->_values, v->_count, sizeof(char*), sx_strcmp);
}

ptrdiff_t vec_str_search(const vec_str* v, const char* s) {
    assert_notnull(v);
    assert_notnull(s);
    if (v->_count) {
        char** p =
            bsearch(&s, v->_values, v->_count, sizeof(char*), sx_strcmp);
        if (p) {
            return p - v->_values;
        }
    }
    return VEC_NOT_FOUND;
}

vec_str vec_str_alloc_split(const char* s, const char* sep) {
    assert_notnull(s);
    assert_notnull(sep);
    ptrdiff_t sep_size = strlen(sep);
    assert(sep_size && "can't split with empty sep");
    vec_str v = vec_str_alloc();
    const char* p = s;
    while (p) {
        const char* q = strstr(p, sep);
        if (q) {
            vec_str_push(&v, strndup(p, q - p));
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

char* vec_str_join(const vec_str* v, const char* sep) {
    assert_notnull(v);
    const ptrdiff_t VEC_SIZE = vec_str_count(v);
    const ptrdiff_t SEP_SIZE = sep ? strlen(sep) : 0;
    ptrdiff_t total_size = 0;
    ptrdiff_t sizes[VEC_SIZE];
    for (ptrdiff_t i = 0; i < VEC_SIZE; ++i) {
        ptrdiff_t size = strlen(vec_str_get(v, i));
        sizes[i] = size;
        total_size += size + SEP_SIZE;
    }
    total_size -= SEP_SIZE; // don't want one at the end
    total_size++;           // allow for \0
    char* s = malloc(total_size);
    assert_alloc(s);
    char* p = s;
    for (ptrdiff_t i = 0; i < VEC_SIZE; ++i) {
        ptrdiff_t size = sizes[i];
        strncpy(p, strndup(vec_str_get(v, i), size), size);
        p += size;
        if (sep && (i + 1 < VEC_SIZE)) { // avoid adding one at the end
            strncpy(p, strndup(sep, SEP_SIZE), SEP_SIZE);
            p += SEP_SIZE;
        }
    }
    *p = 0;
    return s;
}

static void vec_str_grow(vec_str* v) {
    const ptrdiff_t BLOCK_SIZE = 1024 * 1024;
    ptrdiff_t cap =
        (v->_cap < BLOCK_SIZE) ? v->_cap * 2 : v->_cap + BLOCK_SIZE;
    char** p = realloc(v->_values, cap * sizeof(char*));
    assert_alloc(p);
    v->_values = p;
    v->_cap = cap;
}
