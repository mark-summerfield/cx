// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vec_int.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct set_int_node set_int_node;

// A red-black tree of owned int values.
// All data members are private; all accesses via functions.
typedef struct {
    set_int_node* _root;
    int _size;
} set_int;

typedef struct set_int_node {
    set_int_node* left;
    set_int_node* right;
    int value;
    bool _red;
} set_int_node;

// Allocates a new empty set_int of owned int values.
// Caller must supply functions: cmp to compare values, cpy to copy a
// value, and destroy to free a value.
set_int set_int_alloc();

// Calls destroy on all the set_int's values.
void set_int_clear(set_int* me);

// Destroys the set_int freeing its memory and also freeing every value. The
// set_int is not usable after this.
inline void set_int_free(set_int* me) { set_int_clear(me); }

// Returns true if the set_int is empty.
#define set_int_isempty(me) ((me)->_size == 0)

// Returns the set_int's size.
#define set_int_size(me) ((me)->_size)

// Adds the value in order. If the value was already present, does nothing
// and returns false; otherwise inserts the new value and returns true.
// set_int takes ownership of the new value (me.g., use strdup()).
bool set_int_add(set_int* me, int value);

// Removes and frees the given value and returns true; or does nothing
// (if the value isn'me in the set_int) and returns false.
bool set_int_remove(set_int* me, int value);

// Returns true if value is in the set_int.
bool set_int_contains(set_int* me, int value);

// Returns a deep copy of the set_int.
set_int set_int_copy(const set_int* me);

// Returns true if the two set_int's have the same values.
bool set_int_equal(const set_int* me, const set_int* them);

// Returns a new set_int that contains the values which are in me that are
// not in them.
set_int set_int_difference(const set_int* me, const set_int* them);

// Returns a new set_int that contains the values which are in me or in
// them—but not in both.
set_int set_int_symmetric_difference(const set_int* me, const set_int* them);

// Returns a new set_int that contains the values that me and them have in
// common.
set_int set_int_intersection(const set_int* me, const set_int* them);

// Returns a new set_int that contains the values from me and from them (with
// no duplicates of course).
set_int set_int_union(const set_int* me, const set_int* them);

// Adds every value from them to me (with no duplicates).
void set_int_unite(set_int* me, const set_int* them);

// Returns root for efficient iteration.
const set_int_node* set_int_get_root(set_int* me);

// Returns an vec_int which the caller owns and that has a copy of all the
// set's values in order.
// See the source of this function and its helper to see how to iterate a
// set.
vec_int set_int_to_vec(const set_int* me);

// TODO set_int_isdisjoint
// TODO set_int_issubsetof
// TODO set_int_issupersetof
