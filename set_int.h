// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
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

typedef struct {
    int i;
    int size;
    int value;
} set_int_visit_data;

// Allocates a new empty set_int of owned int values.
// Caller must supply functions: cmp to compare values, cpy to copy a
// value, and destroy to free a value.
set_int set_int_alloc();

// Calls destroy on all the set_int's values.
void set_int_clear(set_int* v);

// Destroys the set_int freeing its memory and also freeing every value. The
// set_int is not usable after this.
inline void set_int_free(set_int* v) { set_int_clear(v); }

// Returns true if the set_int is empty.
#define set_int_isempty(v) ((v)->_size == 0)

// Returns the set_int's size.
#define set_int_size(v) ((v)->_size)

// Adds the value in order. If the value was already present, does nothing
// and returns false; otherwise inserts the new value and returns true.
// set_int takes ownership of the new value (e.g., use strdup()).
bool set_int_add(set_int* v, int value);

// Removes and frees the given value and returns true; or does nothing
// (if the value isn'v in the set_int) and returns false.
bool set_int_remove(set_int* v, int value);

// Returns true if value is in the set_int.
bool set_int_contains(set_int* v, int value);

// Returns a deep copy of the set_int.
set_int set_int_copy(const set_int* v);

// Returns true if the two set_int's have the same values.
bool set_int_equal(const set_int* v1, const set_int* v2);

// Calls the given visit function on every node in the tree in order.
// For example, given a set_int, myset:
//  void print_value(const set_int_visit_data* data) {
//      printf("%d/%d: %%s\n", data->i, data->size, data->value);
//  }
//  set_int_visit_all(&myset, print_value);
// TODO add intcontext or int i to apply function?
void set_int_visit_all(const set_int* v,
                       void (*visit)(const set_int_visit_data*));

// Returns a new set_int that contains the values which are in v1 that are
// not in v2.
set_int set_int_difference(const set_int* v1, const set_int* v2);

// Returns a new set_int that contains the values which are in v1 or in
// v2—but not in both.
set_int set_int_symmetric_difference(const set_int* v1, const set_int* v2);

// Returns a new set_int that contains the values that v1 and v2 have in
// common.
set_int set_int_intersection(const set_int* v1, const set_int* v2);

// Returns a new set_int that contains the values from v1 and from v2 (with
// no duplicates of course).
set_int set_int_union(const set_int* v1, const set_int* v2);

// Adds every value from v2 to v1 (with no duplicates).
void set_int_unite(set_int* v1, const set_int* v2);

// Returns root for efficient iteration.
const set_int_node* set_int_root(set_int* v);

// For debugging: caller owns returned int.
const char* set_int_tostring(const set_int* v);

// For debugging: caller owns returned int.
const char* set_int_tostring_range(const set_int* v, int begin, int end);

// TODO set_int_isdisjoint
// TODO set_int_issubsetof
// TODO set_int_issupersetof
