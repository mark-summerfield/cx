#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <stdbool.h>
#include <stdio.h>

// A vector of int values.
// All data members are private; all accesses via functions.
typedef struct vec_int {
    size_t _size; // This is "end", i.e., one past the last value
    size_t _cap;  // The size of the allocated array
    int* _values;
} vec_int;

// Allocates a new empty vec_int with the given capacity.
vec_int vec_int_alloc_cap(size_t cap);

// Allocates a new empty vec_int with a default capacity of 32.
#define vec_int_alloc() vec_int_alloc_cap(32)

// Destroys the vec_int freeing its memory. The vec_int is not usable
// after this.
void vec_int_free(vec_int* v);

// Clears the vec_int freeing its memory. The vec_int is usable after
// this (e.g., push() etc).
void vec_int_clear(vec_int* v);

// Returns true if the vec_int is empty.
#define vec_int_isempty(v) ((v)->_size > 0)

// Returns the vec_int's size.
#define vec_int_size(v) ((v)->_size)

// Returns the vec_int's capacity.
#define vec_int_cap(v) ((v)->_cap)

// Returns the vec_int's int value at position index.
int vec_int_get(const vec_int* v, size_t index);

// Returns the vec_int's int value at its last valid index.
int vec_int_get_last(const vec_int* v);

// Sets the vec_int's value at position index to the given int.
void vec_int_set(vec_int* v, size_t index, int value);

// Inserts the int at position index and moves succeeding values up
// (right), increasing the vec_int's size (and cap if necessary).
void vec_int_insert(vec_int* v, size_t index, int value);

// Sets the vec_int's value at position index to the given int and returns
// the old int value from that position.
int vec_int_replace(vec_int* v, size_t index, int value);

// Removes the value at the given index and closes up the gap.
void vec_int_remove(vec_int* v, size_t index);

// Returns and removes the int value at the given index and closes up the
// gap.
int vec_int_take(vec_int* v, size_t index);

// Removes and returns the last int value. Only use if v.isempty() is false.
int vec_int_pop(vec_int* v);

// Pushes a new int value onto the end of the vec_int, increasing the
// vec_int's size (and cap) if necessary.
void vec_int_push(vec_int* v, int value);

typedef struct {
    size_t index;
    bool found;
} vec_int_find_result;

// Returns the index of the int value in the vec_int and true or 0 and
// false.
vec_int_find_result vec_int_find(const vec_int* v, int value);

// Returns a deep copy of the vec_int.
vec_int vec_int_copy(const vec_int* v);

// Returns true if the two vec_int's have the same int values.
bool vec_int_eq(const vec_int* v1, const vec_int* v2);

// Returns a string of space-separated int values.
// The returned char* value is now owned by the caller.
char* vec_int_tostring(const vec_int* v);

// To iterate:
//  for (size_t i = 0; i < vec_int_size(v); i++)
//      int value = vec_int_get(v, i);
