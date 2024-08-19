// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec.h"
#include <stdlib.h>

static void vec_grow(vec* me);

// This *is* used, via a macro in vec.h; clang-format gets confused.
vec vec_alloc_(vec_alloc_args args) {
    assert(args.cmp && "must provide a cmp function");
    assert(args.cpy && "must provide a cpy function");
    assert(args.destroy && "must provide a destroy function");
    void** values = malloc(args.cap * sizeof(void*));
    assert_alloc(values);
    return (vec){._size = 0,
                 ._cap = args.cap,
                 ._values = values,
                 ._cmp = args.cmp,
                 ._cpy = args.cpy,
                 ._destroy = args.destroy};
}

void vec_free(vec* me) {
    assert_notnull(me);
    vec_clear(me);
    free(me->_values);
    me->_values = NULL;
    me->_cap = 0;
}

void vec_clear(vec* me) {
    assert_notnull(me);
    for (int i = 0; i < me->_size; ++i)
        me->_destroy(me->_values[i]);
    me->_size = 0;
}

const void* vec_get(const vec* me, int index) {
    assert_notnull(me);
    assert_valid_index(me, index);
    return me->_values[index];
}

inline const void* vec_get_last(const vec* me) {
    assert_notnull(me);
    assert_nonempty(me);
    return me->_values[me->_size - 1];
}

void vec_set(vec* me, int index, void* value) {
    assert_notnull(me);
    assert_notnull(value);
    assert_valid_index(me, index);
    me->_destroy(me->_values[index]);
    me->_values[index] = value;
}

void vec_insert(vec* me, int index, void* value) {
    assert_notnull(me);
    assert_notnull(value);
    if (index == me->_size) { // add at the end
        vec_push(me, value);
        return;
    }
    assert_valid_index(me, index);
    if (me->_size == me->_cap)
        vec_grow(me);
    for (int i = me->_size; i > index; --i)
        me->_values[i] = me->_values[i - 1];
    me->_values[index] = value;
    me->_size++;
}

void vec_add(vec* me, void* value) {
    assert_notnull(me);
    assert_notnull(value);
    int high = me->_size - 1;
    if (!me->_size || me->_cmp(&me->_values[high], &value) <= 0) {
        vec_push(me, value); // vec is empty -or- nonempty and value >= high
    } else {
        int low = 0;
        while (low < high) {
            int mid = (low + high) / 2;
            if (me->_cmp(&me->_values[mid], &value) > 0)
                high = mid;
            else
                low = mid + 1;
        }
        vec_insert(me, low, value);
    }
}

void* vec_replace(vec* me, int index, void* value) {
    assert_notnull(me);
    assert_notnull(value);
    assert_valid_index(me, index);
    void* old = me->_values[index];
    me->_values[index] = value;
    return old;
}

inline void vec_remove(vec* me, int index) {
    assert_notnull(me);
    me->_destroy(vec_take(me, index)); // vec_take checks index
}

void* vec_take(vec* me, int index) {
    assert_notnull(me);
    assert_valid_index(me, index);
    void* old = me->_values[index];
    for (int i = index; i < me->_size; ++i)
        me->_values[i] = me->_values[i + 1];
    me->_size--;
    me->_values[me->_size] = NULL;
    return old;
}

void* vec_pop(vec* me) {
    assert_notnull(me);
    assert_nonempty(me);
    return me->_values[--me->_size];
}

void vec_push(vec* me, void* value) {
    assert_notnull(me);
    assert_notnull(value);
    if (me->_size == me->_cap)
        vec_grow(me);
    me->_values[me->_size++] = value;
}

vec vec_copy(const vec* me) {
    assert_notnull(me);
#pragma GCC diagnostic ignored "-Woverride-init"
#pragma GCC diagnostic push
    vec out = vec_alloc(.cap = me->_size ? me->_size : VEC_INITIAL_CAP,
                        .cmp = me->_cmp, .cpy = me->_cpy,
                        .destroy = me->_destroy);
#pragma GCC diagnostic pop
    for (int i = 0; i < me->_size; ++i)
        vec_push(&out, me->_cpy(me->_values[i]));
    return out;
}

void vec_merge(vec* me, vec* them) {
    assert_notnull(me);
    assert_notnull(them);
    assert(me->_cmp == them->_cmp && me->_cpy == them->_cpy &&
           me->_destroy == them->_destroy && "non-matching vecs");
    if ((me->_cap - me->_size) <
        them->_size) { // me doesn't have enough cap
        int cap = me->_size + them->_size;
        void** p = realloc(me->_values, cap * sizeof(void*));
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

bool vec_equal(const vec* me, const vec* them) {
    assert_notnull(me);
    assert_notnull(them);
    if (me->_size != them->_size || me->_cmp != them->_cmp ||
        me->_cpy != them->_cpy || me->_destroy != them->_destroy)
        return false;
    for (int i = 0; i < me->_size; ++i)
        if (me->_cmp(&me->_values[i], &them->_values[i]))
            return false;
    return true;
}

int vec_find(const vec* me, const void* value) {
    assert_notnull(me);
    assert_notnull(value);
    for (int i = 0; i < me->_size; ++i)
        if (me->_cmp(&me->_values[i], &value) == 0)
            return i;
    return VEC_NOT_FOUND;
}

int vec_find_last(const vec* me, const void* value) {
    assert_notnull(me);
    assert_notnull(value);
    for (int i = me->_size - 1; i >= 0; --i)
        if (me->_cmp(&me->_values[i], &value) == 0)
            return i;
    return VEC_NOT_FOUND;
}

void vec_sort(vec* me) {
    assert_notnull(me);
    if (me->_size)
        qsort(me->_values, me->_size, sizeof(void*), me->_cmp);
}

int vec_search(const vec* me, const void* value) {
    assert_notnull(me);
    assert_notnull(value);
    if (me->_size) {
        void** p = bsearch(&value, me->_values, me->_size, sizeof(void*),
                           me->_cmp);
        if (p)
            return p - me->_values;
    }
    return VEC_NOT_FOUND;
}

static void vec_grow(vec* me) {
    const int BLOCK_SIZE = 1024 * 1024;
    int cap =
        (me->_cap < BLOCK_SIZE) ? me->_cap * 2 : me->_cap + BLOCK_SIZE;
    void** p = realloc(me->_values, cap * sizeof(void*));
    assert_alloc(p);
    me->_values = p;
    me->_cap = cap;
}
