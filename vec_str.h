#pragma once
// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <stdbool.h>
#include <stddef.h>

typedef struct vec_str {
    size_t size; // This is "end", i.e., one past the last value
    size_t cap;  // The size of the allocated array
    char** values;
    bool own_values;
} vec_str;

typedef struct {
    size_t cap;
    bool own_values;
} vec_str_alloc_args;

// Allocates a new vec_str with default capacity of 8 that does not own its
// values. Set the initial capacity with .cap and have the vec_str own its
// values with .own_values=true.
#define vec_str_alloc(...)                                                 \
    vec_str_alloc_(                                                        \
        (vec_str_alloc_args){.cap = 8, .own_values = false, __VA_ARGS__})
vec_str vec_str_alloc_(vec_str_alloc_args args);

// Destroys the vec_str freeing its memory and if .own_values is true, also
// freeing every value. The vec_str is not usable after this.
void vec_str_free(vec_str* v);

// Clears the vec_str freeing its memory and if .own_values is true, also
// freeing every value. The vec_str is usable after this (e.g., push() etc).
void vec_str_clear(vec_str* v);

// Returns the vec_str's size.
size_t vec_str_size(vec_str* v) { return v->size; }

// Returns the vec_str's capacity.
size_t vec_str_cap(vec_str* v) { return v->cap; }

// Returns the vec_str's str at position index.
char* vec_str_get(vec_str* v, size_t index);

// Sets the vec_str's str at position index to the given value.
// If own_values is true, frees the old string.
void vec_str_set(vec_str* v, size_t index, char *value);

// Sets the vec_str's str at position index to the given value and
// returns the old str from that position.
char* vec_str_exch(vec_str* v, size_t index, char *value);

// TODO
// value exch(index,value) // insert new value at index; return old value
// void remove(begin,end=0)
// size_t find(value)
// value pop()
// void push(value)
