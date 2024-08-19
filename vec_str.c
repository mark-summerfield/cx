// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str.h"
#include "sx.h"
#include <stdlib.h>
#include <string.h>

static void vec_str_grow(vec_str* me);

vec_str vec_str_alloc_cap(int cap) {
    char** values = malloc(cap * sizeof(char*));
    assert_alloc(values);
    return (vec_str){._size = 0, ._cap = cap, ._values = values};
}

void vec_str_free(vec_str* me) {
    assert_notnull(me);
    vec_str_clear(me);
    free(me->_values);
    me->_values = NULL;
    me->_cap = 0;
}

void vec_str_clear(vec_str* me) {
    assert_notnull(me);
    for (int i = 0; i < me->_size; ++i)
        free(me->_values[i]);
    me->_size = 0;
}

const char* vec_str_get(const vec_str* me, int index) {
    assert_notnull(me);
    assert_valid_index(me, index);
    return me->_values[index];
}

inline const char* vec_str_get_last(const vec_str* me) {
    assert_notnull(me);
    assert_nonempty(me);
    return me->_values[me->_size - 1];
}

void vec_str_set(vec_str* me, int index, char* value) {
    assert_notnull(me);
    assert_notnull(value);
    assert_valid_index(me, index);
    free(me->_values[index]);
    me->_values[index] = value;
}

void vec_str_insert(vec_str* me, int index, char* value) {
    assert_notnull(me);
    assert_notnull(value);
    if (index == me->_size) { // add at the end
        vec_str_push(me, value);
        return;
    }
    assert_valid_index(me, index);
    if (me->_size == me->_cap)
        vec_str_grow(me);
    for (int i = me->_size; i > index; --i)
        me->_values[i] = me->_values[i - 1];
    me->_values[index] = value;
    me->_size++;
}

void vec_str_add(vec_str* me, char* value) {
    assert_notnull(me);
    assert_notnull(value);
    int high = me->_size - 1;
    if (!me->_size || strcmp(me->_values[high], value) <= 0) {
        vec_str_push(me,
                     value); // vec is empty -or- nonempty and value >= high
    } else {
        int low = 0;
        while (low < high) {
            int mid = (low + high) / 2;
            if (strcmp(me->_values[mid], value) > 0)
                high = mid;
            else
                low = mid + 1;
        }
        vec_str_insert(me, low, value);
    }
}

char* vec_str_replace(vec_str* me, int index, char* value) {
    assert_notnull(me);
    assert_notnull(value);
    assert_valid_index(me, index);
    char* old = me->_values[index];
    me->_values[index] = value;
    return old;
}

inline void vec_str_remove(vec_str* me, int index) {
    assert_notnull(me);
    free(vec_str_take(me, index));
}

char* vec_str_take(vec_str* me, int index) {
    assert_notnull(me);
    assert_valid_index(me, index);
    char* old = me->_values[index];
    for (int i = index; i < me->_size; ++i)
        me->_values[i] = me->_values[i + 1];
    me->_size--;
    me->_values[me->_size] = NULL;
    return old;
}

char* vec_str_pop(vec_str* me) {
    assert_notnull(me);
    assert_nonempty(me);
    return me->_values[--me->_size];
}

void vec_str_push(vec_str* me, char* value) {
    assert_notnull(me);
    assert_notnull(value);
    if (me->_size == me->_cap)
        vec_str_grow(me);
    me->_values[me->_size++] = value;
}

vec_str vec_str_copy(const vec_str* me) {
    assert_notnull(me);
    vec_str out =
        vec_str_alloc_cap(me->_size ? me->_size : VEC_INITIAL_CAP);
    for (int i = 0; i < me->_size; ++i)
        vec_str_push(&out, strdup(me->_values[i]));
    return out;
}

void vec_str_merge(vec_str* me, vec_str* them) {
    assert_notnull(me);
    assert_notnull(them);
    if ((me->_cap - me->_size) <
        them->_size) { // me doesn't have enough cap
        int cap = me->_size + them->_size;
        char** p = realloc(me->_values, cap * sizeof(char*));
        assert_alloc(p);
        me->_values = p;
        me->_cap = cap;
    }
    for (int i = 0; i < them->_size; ++i)
        me->_values[me->_size++] = them->_values[i]; // push
    free(them->_values);
    them->_values = NULL;
    them->_cap = 0;
    them->_size = 0;
}

bool vec_str_equal(const vec_str* me, const vec_str* them) {
    assert_notnull(me);
    assert_notnull(them);
    for (int i = 0; i < me->_size; ++i)
        if (strcmp(me->_values[i], them->_values[i]))
            return false;
    return true;
}

int vec_str_find(const vec_str* me, const char* value) {
    assert_notnull(me);
    assert_notnull(value);
    for (int i = 0; i < me->_size; ++i)
        if (strcmp(me->_values[i], value) == 0)
            return i;
    return VEC_NOT_FOUND;
}

int vec_str_find_last(const vec_str* me, const char* value) {
    assert_notnull(me);
    assert_notnull(value);
    for (int i = me->_size - 1; i >= 0; --i)
        if (strcmp(me->_values[i], value) == 0)
            return i;
    return VEC_NOT_FOUND;
}

void vec_str_sort(vec_str* me) {
    assert_notnull(me);
    if (me->_size)
        qsort(me->_values, me->_size, sizeof(char*), sx_strcmp);
}

int vec_str_search(const vec_str* me, const char* s) {
    assert_notnull(me);
    assert_notnull(s);
    if (me->_size) {
        char** p =
            bsearch(&s, me->_values, me->_size, sizeof(char*), sx_strcmp);
        if (p)
            return p - me->_values;
    }
    return VEC_NOT_FOUND;
}

vec_str vec_str_alloc_split(const char* s, const char* sep) {
    assert_notnull(s);
    assert_notnull(sep);
    int sep_size = strlen(sep);
    assert(sep_size && "can't split with empty sep");
    vec_str out = vec_str_alloc();
    const char* p = s;
    while (p) {
        const char* q = strstr(p, sep);
        if (q) {
            vec_str_push(&out, strndup(p, q - p));
            p = q + sep_size;
        } else {
            if (strlen(p))
                vec_str_push(&out, strdup(p));
            break;
        }
    }
    return out;
}

char* vec_str_join(const vec_str* me, const char* sep) {
    assert_notnull(me);
    const int VEC_SIZE = vec_str_size(me);
    const int SEP_SIZE = sep ? strlen(sep) : 0;
    int total_size = 0;
    int sizes[VEC_SIZE];
    for (int i = 0; i < VEC_SIZE; ++i) {
        int size = strlen(vec_str_get(me, i));
        sizes[i] = size;
        total_size += size + SEP_SIZE;
    }
    total_size -= SEP_SIZE; // don't want one at the end
    total_size++;           // allow for \0
    char* out = malloc(total_size);
    assert_alloc(out);
    char* p = out;
    for (int i = 0; i < VEC_SIZE; ++i) {
        int size = sizes[i];
        strncpy(p, strndup(vec_str_get(me, i), size), size);
        p += size;
        if (sep && (i + 1 < VEC_SIZE)) { // avoid adding one at the end
            strncpy(p, strndup(sep, SEP_SIZE), SEP_SIZE);
            p += SEP_SIZE;
        }
    }
    *p = 0;
    return out;
}

static void vec_str_grow(vec_str* me) {
    const int BLOCK_SIZE = 1024 * 1024;
    int cap =
        (me->_cap < BLOCK_SIZE) ? me->_cap * 2 : me->_cap + BLOCK_SIZE;
    char** p = realloc(me->_values, cap * sizeof(char*));
    assert_alloc(p);
    me->_values = p;
    me->_cap = cap;
}
