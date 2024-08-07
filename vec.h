#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <stdbool.h>
#include <stddef.h>

typedef struct vec {
    size_t size; // This is "end", i.e., one past the last value
    size_t cap;  // The size of the allocated array
    void** values;
    bool (*eq)(void* a, void* b);
    void (*destroy)(void* values);
} vec;

typedef struct {
    size_t cap;
    bool (*eq)(void* a, void* b);
    void (*destroy)(void* values);
} vec_alloc_args;

// Allocates a new vec with default capacity of 8.
// Set the initial capacity with .cap.
// Caller must supply an eq function.
// Values must be pointers, and the caller must supply a destroy function
// (since vec owns its values).
#define vec_alloc(...) vec_alloc_((vec_alloc_args){.cap = 8, __VA_ARGS__})
vec vec_alloc_(vec_alloc_args args);

// Destroys the vec freeing its memory and also freeing every value. The vec
// is not usable after this.
void vec_free(vec* v);

// Clears the vec freeing its memory and also freeing every value. The vec
// is usable after this (e.g., push() etc).
void vec_clear(vec* v);

// Returns true if the vec is empty.
#define vec_isempty(v) ((v)->size > 0)

// Returns the vec's size.
#define vec_size(v) ((v)->size)

// Returns the vec's capacity.
#define vec_cap(v) ((v)->cap)

// Returns the vec's value at position index.
// vec retains ownership, so do not delete the value.
const void* vec_get(vec* v, size_t index);

// Sets the vec's value at position index to the given value.
// vec takes ownership of the new value (e.g., if char* then use strdup())
// and frees the old value.
void vec_set(vec* v, size_t index, void* value);

// Sets the vec's value at position index to the given value and returns
// the old value from that position.
// vec takes ownership of the new value (e.g., if char* then use strdup()).
// The returned value is now owned by the caller.
void* vec_replace(vec* v, size_t index, void* value);

// Removes and frees the value at the given index and closes up the gap.
void vec_remove(vec* v, size_t index);

// Returns and removes the value at the given index and closes up the
// gap.
// The returned value is now owned by the caller.
void* vec_take(vec* v, size_t index);

// Removes and returns the last value. Only use if v.isempty() is false.
// The returned value is now owned by the caller.
void* vec_pop(vec* v);

// Pushes a new value onto the end of the vec, increasing the vec's size
// (and cap) if necessary.
// vec takes ownership of the value (e.g., if char* then use strdup()).
void vec_push(vec* v, void* value);

typedef struct {
    size_t index;
    bool found;
} vec_find_result;

// Returns the index of value in the vec and true or 0 and false.
vec_find_result vec_find(vec* v, void* value);

// Returns a deep copy of the vec including eq and destroy using the
// given cp function for copying each value.
vec vec_copy(vec* v, void* (*cp)(void*));

// Returns true if the two vec's have the same values and eq and destroy
// functions.
bool vec_eq(vec* v1, vec* v2);

// To iterate:
//  for (size_t i = 0; i < v.size(); i++) void* value = v.get(i);
