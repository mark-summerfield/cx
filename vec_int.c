// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_int.h"
#include "cx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void vec_int_grow(vec_int* me);

vec_int vec_int_alloc_cap(int cap) {
    cap = cap ? cap : 32;
    int* values = malloc(cap * sizeof(int));
    assert_alloc(values);
    return (vec_int){._size = 0, ._cap = cap, ._values = values};
}

void vec_int_free(vec_int* me) {
    assert_notnull(me);
    free(me->_values);
    me->_values = NULL;
    me->_size = 0;
    me->_cap = 0;
}

inline void vec_int_clear(vec_int* me) { me->_size = 0; }

int vec_int_get(const vec_int* me, int index) {
    assert_notnull(me);
    assert_valid_index(me, index);
    return me->_values[index];
}

inline int vec_int_get_last(const vec_int* me) {
    assert_notnull(me);
    assert_nonempty(me);
    return me->_values[me->_size - 1];
}

void vec_int_set(vec_int* me, int index, int value) {
    assert_notnull(me);
    assert_valid_index(me, index);
    me->_values[index] = value;
}

void vec_int_insert(vec_int* me, int index, int value) {
    assert_notnull(me);
    if (index == me->_size) { // add at the end
        vec_int_push(me, value);
        return;
    }
    assert_valid_index(me, index);
    if (me->_size == me->_cap)
        vec_int_grow(me);
    for (int i = me->_size; i > index; --i)
        me->_values[i] = me->_values[i - 1];
    me->_values[index] = value;
    me->_size++;
}

void vec_int_add(vec_int* me, int value) {
    assert_notnull(me);
    assert_notnull(value);
    int high = me->_size - 1;
    if (!me->_size || me->_values[high] <= value) {
        vec_int_push(me,
                     value); // vec is empty -or- nonempty and value >= high
    } else {
        int low = 0;
        while (low < high) {
            int mid = (low + high) / 2;
            if (me->_values[mid] > value)
                high = mid;
            else
                low = mid + 1;
        }
        vec_int_insert(me, low, value);
    }
}

int vec_int_replace(vec_int* me, int index, int value) {
    assert_notnull(me);
    assert_valid_index(me, index);
    int old = me->_values[index];
    me->_values[index] = value;
    return old;
}

void vec_int_remove(vec_int* me, int index) {
    assert_notnull(me);
    assert_valid_index(me, index);
    for (int i = index; i < me->_size; ++i)
        me->_values[i] = me->_values[i + 1];
    me->_size--;
}

int vec_int_take(vec_int* me, int index) {
    assert_notnull(me);
    assert_valid_index(me, index);
    int old = me->_values[index];
    vec_int_remove(me, index);
    return old;
}

int vec_int_pop(vec_int* me) {
    assert_notnull(me);
    assert_nonempty(me);
    return me->_values[--me->_size];
}

void vec_int_push(vec_int* me, int value) {
    assert_notnull(me);
    if (me->_size == me->_cap)
        vec_int_grow(me);
    me->_values[me->_size++] = value;
}

vec_int vec_int_copy(const vec_int* me) {
    assert_notnull(me);
    vec_int out = vec_int_alloc_cap(me->_size);
    for (int i = 0; i < me->_size; ++i)
        vec_int_push(&out, me->_values[i]);
    return out;
}

void vec_int_merge(vec_int* me, vec_int* them) {
    assert_notnull(me);
    assert_notnull(them);
    if ((me->_cap - me->_size) <
        them->_size) { // me doesn't have enough cap
        int cap = me->_size + them->_size;
        int* p = realloc(me->_values, cap * sizeof(int));
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

bool vec_int_equal(const vec_int* me, const vec_int* them) {
    assert_notnull(me);
    assert_notnull(them);
    if (me->_size != them->_size)
        return false;
    for (int i = 0; i < me->_size; ++i)
        if (me->_values[i] != them->_values[i])
            return false;
    return true;
}

int vec_int_find(const vec_int* me, int value) {
    assert_notnull(me);
    for (int i = 0; i < me->_size; ++i)
        if (me->_values[i] == value)
            return i;
    return VEC_NOT_FOUND;
}

int vec_int_find_last(const vec_int* me, int value) {
    assert_notnull(me);
    for (int i = me->_size - 1; i >= 0; --i)
        if (me->_values[i] == value)
            return i;
    return VEC_NOT_FOUND;
}

int intcmp(const void* a, const void* b) { return (*(int*)a - *(int*)b); }

void vec_int_sort(vec_int* me) {
    assert_notnull(me);
    if (me->_size)
        qsort(me->_values, me->_size, sizeof(int), intcmp);
}

int vec_int_search(const vec_int* me, int i) {
    assert_notnull(me);
    if (me->_size) {
        const int* p =
            bsearch(&i, me->_values, me->_size, sizeof(int), intcmp);
        if (p)
            return p - me->_values;
    }
    return VEC_NOT_FOUND;
}

char* vec_int_tostring(const vec_int* me) {
    assert_notnull(me);
    const int BUF_SIZE = 128;
    const int VEC_SIZE = vec_int_size(me);
    int cap = VEC_SIZE * 4;
    char* out = malloc(cap);
    assert_alloc(out);
    int pos = 0;
    char buf[BUF_SIZE];
    for (int i = 0; i < VEC_SIZE; ++i) {
        size_t n = snprintf(buf, BUF_SIZE, "%d ", vec_int_get(me, i));
        strncpy(&out[pos], buf, n);
        pos += n;
        if (pos + 4 > cap) {
            cap *= 2;
            char* p = realloc(out, cap);
            assert_alloc(p);
            out = p;
        }
    }
    out[pos - 1] = 0; // avoid trailing space
    return out;
}

static void vec_int_grow(vec_int* me) {
    const int BLOCK_SIZE = 1024 * 1024;
    int cap =
        (me->_cap < BLOCK_SIZE) ? me->_cap * 2 : me->_cap + BLOCK_SIZE;
    int* p = realloc(me->_values, cap * sizeof(int));
    assert_alloc(p);
    me->_values = p;
    me->_cap = cap;
}
