#pragma once
// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <stdbool.h>
#include <stddef.h>

typedef struct vec {
    size_t size; // This is "end", i.e., one past the last value
    size_t cap;  // The size of the allocated array
    void* values;
    bool own_values;
} vec;

typedef struct {
    size_t cap;
    bool own_values;
} vec_alloc_args;

// Allocates a new vec with default capacity of 8 that does not own its
// values. Set the initial capacity with .cap and have the vec own its
// values with .own_values=true.
#define vec_alloc(...)                                                     \
    vec_alloc_((vec_alloc_args){.cap = 8, .own_values = false, __VA_ARGS__})
vec vec_alloc_(vec_alloc_args args);

// Destroys the vec freeing its memory and if .own_values is true, also
// freeing every value. The vec is not usable after this.
void vec_free(vec* v);

// Clears the vec freeing its memory and if .own_values is true, also
// freeing every value. The vec is usable after this (e.g., push() etc).
void vec_clear(vec* v);

// Returns the vec's size.
size_t vec_size(vec* v) { return v->size; }

// Returns the vec's capacity.
size_t vec_cap(vec* v) { return v->cap; }

// TODO
// value get(index)
// set(index,value) // if own_values then delete old
// value exch(index,value) // insert new value at index; return old value
// void remove(begin,end=0)
// size_t find(value)
// value pop()
// void push(value)
