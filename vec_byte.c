// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_byte.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void vec_byte_grow(VecByte* vec);

VecByte vec_byte_alloc_cap(int cap) {
    cap = cap > 0 ? cap : 0;
    byte* values = NULL;
    if (cap) {
        values = malloc(cap * sizeof(byte));
        assert_alloc(values);
    }
    return (VecByte){._size = 0, ._cap = cap, .raw = values};
}

void vec_byte_free(VecByte* vec) {
    assert_notnull(vec);
    free(vec->raw);
    vec->raw = NULL;
    vec->_size = 0;
    vec->_cap = 0;
}

inline void vec_byte_clear(VecByte* vec) { vec->_size = 0; }

byte vec_byte_get(const VecByte* vec, int index) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    return vec->raw[index];
}

inline byte vec_byte_get_first(const VecByte* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->raw[0];
}

inline byte vec_byte_get_last(const VecByte* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->raw[vec->_size - 1];
}

void vec_byte_set(VecByte* vec, int index, byte value) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    vec->raw[index] = value;
}

void vec_byte_insert(VecByte* vec, int index, byte value) {
    assert_notnull(vec);
    assert_valid_index(vec, index);
    if (vec->_size == vec->_cap)
        vec_byte_grow(vec);
    for (int i = vec->_size; i > index; --i)
        vec->raw[i] = vec->raw[i - 1];
    vec->raw[index] = value;
    vec->_size++;
}

byte vec_byte_replace(VecByte* vec, int index, byte value) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    byte old = vec->raw[index];
    vec->raw[index] = value;
    return old;
}

void vec_byte_remove(VecByte* vec, int index) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    for (int i = index; i < vec->_size; ++i)
        vec->raw[i] = vec->raw[i + 1];
    vec->_size--;
}

byte vec_byte_take(VecByte* vec, int index) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    byte old = vec->raw[index];
    vec_byte_remove(vec, index);
    return old;
}

byte vec_byte_pop(VecByte* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->raw[--vec->_size];
}

void vec_byte_push(VecByte* vec, byte value) {
    assert_notnull(vec);
    if (vec->_size == vec->_cap)
        vec_byte_grow(vec);
    vec->raw[vec->_size++] = value;
}

VecByte vec_byte_copy(const VecByte* vec) {
    assert_notnull(vec);
    VecByte out = vec_byte_alloc_cap(vec->_size);
    for (int i = 0; i < vec->_size; ++i)
        vec_byte_push(&out, vec->raw[i]);
    return out;
}

void vec_byte_merge(VecByte* vec1, VecByte* vec2) {
    assert_notnull(vec1);
    assert_notnull(vec2);
    if ((vec1->_cap - vec1->_size) <
        vec2->_size) { // vec1 doesn't have enough cap
        int cap = vec1->_size + vec2->_size;
        byte* p = realloc(vec1->raw, cap * sizeof(byte));
        assert_alloc(p);
        vec1->raw = p;
        vec1->_cap = cap;
    }
    for (int i = 0; i < vec2->_size; ++i)
        vec1->raw[vec1->_size++] = vec2->raw[i]; // push
    vec_byte_free(vec2);
}

bool vec_byte_equal(const VecByte* vec1, const VecByte* vec2) {
    assert_notnull(vec1);
    assert_notnull(vec2);
    if (vec1->_size != vec2->_size)
        return false;
    if (!vec1->_size)
        return true; // both empty
    return memcmp(vec1->raw, vec2->raw, sizeof(byte) * vec1->_size) == 0;
}

int vec_byte_find(const VecByte* vec, byte value) {
    assert_notnull(vec);
    for (int i = 0; i < vec->_size; ++i)
        if (vec->raw[i] == value)
            return i;
    return VEC_NOT_FOUND;
}

int vec_byte_find_last(const VecByte* vec, byte value) {
    assert_notnull(vec);
    for (int i = vec->_size - 1; i >= 0; --i)
        if (vec->raw[i] == value)
            return i;
    return VEC_NOT_FOUND;
}

char* vec_byte_to_str(const VecByte* vec) {
    if (!vec->_size)
        return NULL;
    int size = (vec->_size * 3) + 1; // 2 hex digits + space per byte
    char* s = malloc(size);
    assert_alloc(s);
    s[0] = 0;
    char* p = s;
    for (int i = 0; i < vec->_size; ++i) {
        int n = sprintf(p, "%02X ", vec->raw[i]);
        p += n;
    }
    *--p = 0;
    return s;
}

void vec_byte_dump(const VecByte* vec) {
    if (vec->_size)
        for (int i = 0; i < vec->_size; ++i)
            printf("[%d]=%02X\n", i, vec->raw[i]);
    else
        printf("(empty)");
}

static void vec_byte_grow(VecByte* vec) {
    int cap = vec->_cap;
    assert((!cap && !vec->raw) || (cap && vec->raw));
    vec->_cap = GROW_CAP(cap);
    vec->raw = realloc(vec->raw, vec->_cap * sizeof(byte));
    assert_alloc(vec->raw);
}
