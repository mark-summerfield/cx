// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str.h"
#include "sx.h"
#include <stdlib.h>
#include <string.h>

static void vec_str_grow(VecStr* vec);

VecStr vec_str_alloc_custom(int cap, bool owns) {
    cap = cap > 0 ? cap : VEC_INITIAL_CAP;
    char** values = malloc(cap * sizeof(char*));
    assert_alloc(values);
    return (VecStr){
        ._size = 0, ._cap = cap, ._owns = owns, ._values = values};
}

void vec_str_free(VecStr* vec) {
    assert_notnull(vec);
    vec_str_clear(vec);
    free(vec->_values);
    vec->_values = NULL;
    vec->_cap = 0;
}

void vec_str_clear(VecStr* vec) {
    assert_notnull(vec);
    if (vec->_owns)
        for (int i = 0; i < vec->_size; ++i)
            free(vec->_values[i]);
    vec->_size = 0;
}

char* vec_str_get(const VecStr* vec, int index) {
    assert_notnull(vec);
    assert_valid_index(vec, index);
    return vec->_values[index];
}

inline char* vec_str_get_last(const VecStr* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->_values[vec->_size - 1];
}

void vec_str_set(VecStr* vec, int index, char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    assert_valid_index(vec, index);
    if (vec->_owns)
        free(vec->_values[index]);
    vec->_values[index] = value;
}

void vec_str_insert(VecStr* vec, int index, char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    if (index == vec->_size) { // add at the end
        vec_str_push(vec, value);
        return;
    }
    assert_valid_index(vec, index);
    if (vec->_size == vec->_cap)
        vec_str_grow(vec);
    for (int i = vec->_size; i > index; --i)
        vec->_values[i] = vec->_values[i - 1];
    vec->_values[index] = value;
    vec->_size++;
}

void vec_str_add(VecStr* vec, char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    int high = vec->_size - 1;
    if (!vec->_size || strcmp(vec->_values[high], value) <= 0) {
        vec_str_push(vec,
                     value); // vec is empty -or- nonempty and value >= high
    } else {
        int low = 0;
        while (low < high) {
            int mid = (low + high) / 2;
            if (strcmp(vec->_values[mid], value) > 0)
                high = mid;
            else
                low = mid + 1;
        }
        vec_str_insert(vec, low, value);
    }
}

char* vec_str_replace(VecStr* vec, int index, char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    assert_valid_index(vec, index);
    char* old = vec->_values[index];
    vec->_values[index] = value;
    return old;
}

inline void vec_str_remove(VecStr* vec, int index) {
    assert_notnull(vec);
    char* old = vec_str_take(vec, index);
    if (vec->_owns)
        free(old);
}

char* vec_str_take(VecStr* vec, int index) {
    assert_notnull(vec);
    assert_valid_index(vec, index);
    char* old = vec->_values[index];
    for (int i = index; i < vec->_size; ++i)
        vec->_values[i] = vec->_values[i + 1];
    vec->_size--;
    vec->_values[vec->_size] = NULL;
    return old;
}

char* vec_str_pop(VecStr* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->_values[--vec->_size];
}

void vec_str_push(VecStr* vec, char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    if (vec->_size == vec->_cap)
        vec_str_grow(vec);
    vec->_values[vec->_size++] = value;
}

VecStr vec_str_copy(const VecStr* vec) {
    assert_notnull(vec);
    VecStr out = vec_str_alloc_custom(
        vec->_size ? vec->_size : VEC_INITIAL_CAP, vec->_owns);
    for (int i = 0; i < vec->_size; ++i) {
        char* value = vec->_values[i];
        if (vec->_owns)
            value = strdup(value);
        vec_str_push(&out, value);
    }
    return out;
}

void vec_str_merge(VecStr* vec1, VecStr* vec2) {
    assert_notnull(vec1);
    assert_notnull(vec2);
    if ((vec1->_cap - vec1->_size) <
        vec2->_size) { // vec1 doesn't have enough cap
        int cap = vec1->_size + vec2->_size;
        char** p = realloc(vec1->_values, cap * sizeof(char*));
        assert_alloc(p);
        vec1->_values = p;
        vec1->_cap = cap;
    }
    for (int i = 0; i < vec2->_size; ++i)
        vec1->_values[vec1->_size++] = vec2->_values[i]; // push
    free(vec2->_values);
    vec2->_values = NULL;
    vec2->_cap = 0;
    vec2->_size = 0;
}

bool vec_str_equal(const VecStr* vec1, const VecStr* vec2) {
    assert_notnull(vec1);
    assert_notnull(vec2);
    for (int i = 0; i < vec1->_size; ++i)
        if (strcmp(vec1->_values[i], vec2->_values[i]))
            return false;
    return true;
}

int vec_str_find(const VecStr* vec, const char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    for (int i = 0; i < vec->_size; ++i)
        if (strcmp(vec->_values[i], value) == 0)
            return i;
    return VEC_NOT_FOUND;
}

int vec_str_find_last(const VecStr* vec, const char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    for (int i = vec->_size - 1; i >= 0; --i)
        if (strcmp(vec->_values[i], value) == 0)
            return i;
    return VEC_NOT_FOUND;
}

void vec_str_sort(VecStr* vec) {
    assert_notnull(vec);
    if (vec->_size)
        qsort(vec->_values, vec->_size, sizeof(char*), sx_strcmp);
}

int vec_str_search(const VecStr* vec, const char* s) {
    assert_notnull(vec);
    assert_notnull(s);
    if (vec->_size) {
        char** p =
            bsearch(&s, vec->_values, vec->_size, sizeof(char*), sx_strcmp);
        if (p)
            return p - vec->_values;
    }
    return VEC_NOT_FOUND;
}

VecStr vec_str_alloc_split(const char* s, const char* sep) {
    assert_notnull(s);
    assert_notnull(sep);
    int sep_size = strlen(sep);
    assert(sep_size && "can't split with empty sep");
    VecStr vec = vec_str_alloc();
    const char* p = s;
    while (p) {
        const char* q = strstr(p, sep);
        if (q) {
            vec_str_push(&vec, strndup(p, q - p));
            p = q + sep_size;
        } else {
            if (strlen(p))
                vec_str_push(&vec, strdup(p));
            break;
        }
    }
    return vec;
}

char* vec_str_join(const VecStr* vec, const char* sep) {
    assert_notnull(vec);
    const int VEC_SIZE = vec->_size;
    if (!VEC_SIZE)
        return NULL; // empty
    const int SEP_SIZE = sep ? strlen(sep) : 0;
    int total_size = 0;
    int sizes[VEC_SIZE];
    for (int i = 0; i < VEC_SIZE; ++i) {
        int size = strlen(vec->_values[i]);
        sizes[i] = size;
        total_size += size;
    }
    total_size += ((VEC_SIZE - 1) * SEP_SIZE) + 1; // +1 for 0-terminator
    char* s = malloc(total_size);
    assert_alloc(s);
    char* p = s;
    for (int i = 0; i < VEC_SIZE; ++i) {
        int size = sizes[i];
        strncpy(p, vec->_values[i], size);
        p += size;
        if (sep && (i + 1 < VEC_SIZE)) { // avoid adding sep at the end
            strncpy(p, sep, SEP_SIZE);
            p += SEP_SIZE;
        }
    }
    *p = 0;
    return s;
}

static void vec_str_grow(VecStr* vec) {
    const int BLOCK_SIZE = 1024 * 1024;
    int cap =
        (vec->_cap < BLOCK_SIZE) ? vec->_cap * 2 : vec->_cap + BLOCK_SIZE;
    char** p = realloc(vec->_values, cap * sizeof(char*));
    assert_alloc(p);
    vec->_values = p;
    vec->_cap = cap;
}
