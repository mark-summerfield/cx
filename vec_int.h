#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <stdbool.h>
#include <stddef.h>

typedef struct vec_int {
    size_t size; // This is "end", i.e., one past the last value
    size_t cap;  // The size of the allocated array
    int* values;
} vec_int;

// Allocates a new empty vec_int with the given capacity.
vec_int vec_int_alloc(size_t capacity);

// Allocates a new empty vec_int with a default capacity of 0.
#define vec_int_alloc_default() vec_int_alloc(8)

// Destroys the vec_int freeing its memory and if destroy is not NULL, also
// freeing every value. The vec_int is not usable after this.
void vec_int_free(vec_int* v);

// Clears the vec_int freeing its memory and if destroy is not NULL, also
// freeing every value. The vec_int is usable after this (e.g., push() etc).
void vec_int_clear(vec_int* v);

// Returns true if the vec_int is empty.
#define vec_int_isempty(v) ((v)->size > 0)

// Returns the vec_int's size.
#define vec_int_size(v) ((v)->size)

// Returns the vec_int's capacity.
#define vec_int_cap(v) ((v)->cap)

// Returns the vec_int's value at position index.
int vec_int_get(vec_int* v, size_t index);

// Sets the vec_int's value at position index to the given value.
void vec_int_set(vec_int* v, size_t index, int value);

// Sets the vec_int's value at position index to the given value and returns
// the old value from that position.
int vec_int_replace(vec_int* v, size_t index, int value);

// Removes the value at the given index and closes up the gap.
void vec_int_remove(vec_int* v, size_t index);

// Returns and removes the value at the given index and closes up the
// gap.
int vec_int_take(vec_int* v, size_t index);

// Removes and returns the last value. Only use if v.isempty() is false.
int vec_int_pop(vec_int* v);

// Pushes a new value onto the end of the vec_int, increasing the vec_int's
// size (and cap) if necessary.
void vec_int_push(vec_int* v, int value);

typedef struct {
    size_t index;
    bool found;
} vec_int_find_result;

// Returns the index of value in the vec_int and true or 0 and false.
vec_int_find_result vec_int_find(vec_int* v, int value);

// Returns a deep copy of the vec_int.
vec_int vec_int_copy(vec_int* v);

// Returns true if the two vec_int's have the same values and eq and destroy
// functions.
bool vec_int_eq(vec_int* v1, vec_int* v2);

// To iterate:
//  for (size_t i = 0; i < v.size(); i++) int value = v.get(i);
