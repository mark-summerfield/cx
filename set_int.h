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
    set_int_node* _left;
    set_int_node* _right;
    int value;
    bool _red;
} set_int_node;

// Allocates a new empty set_int of owned int values.
// Caller must supply functions: cmp to compare values, cpy to copy a
// value, and destroy to free a value.
set_int set_int_alloc();

// Calls destroy on all the set_int's values.
void set_int_clear(set_int* e);

// Destroys the set_int freeing its memory and also freeing every value. The
// set_int is not usable after this.
inline void set_int_free(set_int* e) { set_int_clear(e); }

// Returns true if the set_int is empty.
#define set_int_isempty(e) ((e)->_size == 0)

// Returns the set_int's size.
#define set_int_size(e) ((e)->_size)

// Adds the value in order. If the value was already present, does nothing
// and returns false; otherwise inserts the new value and returns true.
// set_int takes ownership of the new value (e.g., use strdup()).
bool set_int_add(set_int* e, int value);

// Removes and frees the given value and returns true; or does nothing
// (if the value isn'e in the set_int) and returns false.
bool set_int_remove(set_int* e, int value);

// Returns true if value is in the set_int.
bool set_int_contains(set_int* e, int value);

// Returns a deep copy of the set_int.
set_int set_int_copy(const set_int* e);

// Returns true if the two set_int's have the same values.
bool set_int_equal(const set_int* e1, const set_int* e2);

// Returns a new set_int that contains the values which are in e1 that are
// not in e2.
set_int set_int_difference(const set_int* e1, const set_int* e2);

// Returns a new set_int that contains the values which are in e1 or in
// e2—but not in both.
set_int set_int_symmetric_difference(const set_int* e1, const set_int* e2);

// Returns a new set_int that contains the values that e1 and e2 have in
// common.
set_int set_int_intersection(const set_int* e1, const set_int* e2);

// Returns a new set_int that contains the values from e1 and from e2 (with
// no duplicates of course).
set_int set_int_union(const set_int* e1, const set_int* e2);

// Adds every value from e2 to e1 (with no duplicates).
void set_int_unite(set_int* e1, const set_int* e2);

// Returns root for efficient iteration.
const set_int_node* set_int_get_root(set_int* e);

// Returns an vec_int copy of all the set's values in order.
vec_int set_int_to_vec(const set_int* e);

// TODO set_int_isdisjoint
// TODO set_int_issubsetof
// TODO set_int_issupersetof
