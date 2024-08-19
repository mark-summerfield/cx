// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct set_str_node set_str_node;

// A red-black tree of owned char* values.
// All data members are private; all accesses via functions.
typedef struct {
    set_str_node* _root;
    int _size;
} set_str;

typedef struct set_str_node {
    set_str_node* _left;
    set_str_node* _right;
    char* value;
    bool _red;
} set_str_node;

typedef struct {
    int i;
    int size;
    char* value;
} set_str_visit_data;

// Allocates a new empty set_str of owned char* values.
// Caller must supply functions: cmp to compare values, cpy to copy a
// value, and destroy to free a value.
set_str set_str_alloc();

// Calls destroy on all the set_str's values.
void set_str_clear(set_str* v);

// Destroys the set_str freeing its memory and also freeing every value. The
// set_str is not usable after this.
inline void set_str_free(set_str* v) { set_str_clear(v); }

// Returns true if the set_str is empty.
#define set_str_isempty(v) ((v)->_size == 0)

// Returns the set_str's size.
#define set_str_size(v) ((v)->_size)

// Adds the value in order. If the value was already present, does nothing
// and returns false; otherwise inserts the new value and returns true.
// set_str takes ownership of the new value (e.g., use strdup()).
bool set_str_add(set_str* v, char* value);

// Removes and frees the given value and returns true; or does nothing
// (if the value isn'v in the set_str) and returns false.
bool set_str_remove(set_str* v, const char* value);

// Returns true if value is in the set_str.
bool set_str_contains(set_str* v, const char* value);

// Returns a deep copy of the set_str.
set_str set_str_copy(const set_str* v);

// Returns true if the two set_str's have the same values.
bool set_str_equal(const set_str* v1, const set_str* v2);

// Calls the given visit function on every node in the tree in order.
// For example, given a set_str, myset:
//  void print_value(const set_str_visit_data* data) {
//      printf("%d/%d: %%s\n", data->i, data->size, data->value);
//  }
//  set_str_visit_all(&myset, print_value);
// TODO add char*context or int i to apply function?
void set_str_visit_all(const set_str* v,
                       void (*visit)(const set_str_visit_data*));

// Returns a new set_str that contains the values which are in v1 that are
// not in v2.
set_str set_str_difference(const set_str* v1, const set_str* v2);

// Returns a new set_str that contains the values which are in v1 or in
// v2—but not in both.
set_str set_str_symmetric_difference(const set_str* v1, const set_str* v2);

// Returns a new set_str that contains the values that v1 and v2 have in
// common.
set_str set_str_intersection(const set_str* v1, const set_str* v2);

// Returns a new set_str that contains the values from v1 and from v2 (with
// no duplicates of course).
set_str set_str_union(const set_str* v1, const set_str* v2);

// Adds every value from v2 to v1 (with no duplicates).
void set_str_unite(set_str* v1, const set_str* v2);

// Returns root for efficient iteration.
const set_str_node* set_str_root(set_str* v);

// For debugging: caller owns returned char*.
const char* set_str_tostring(const set_str* v);

// For debugging: caller owns returned char*.
const char* set_str_tostring_range(const set_str* v, int begin, int end);

// TODO set_str_isdisjoint
// TODO set_str_issubsetof
// TODO set_str_issupersetof
