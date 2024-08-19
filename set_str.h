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
    SSIZE_T _size;
} set_str;

typedef struct set_str_node {
    char* value;
    set_str_node* _left;
    set_str_node* _right;
    bool _red;
} set_str_node;

typedef struct {
    char* value;
    SSIZE_T i;
    SSIZE_T size;
} set_str_visit_data;

// Allocates a new empty set_str of owned char* values.
// Caller must supply functions: cmp to compare values, cpy to copy a
// value, and destroy to free a value.
set_str set_str_alloc();

// Calls destroy on all the set_str's values.
void set_str_clear(set_str* t);

// Destroys the set_str freeing its memory and also freeing every value. The
// set_str is not usable after this.
inline void set_str_free(set_str* t) { set_str_clear(t); }

// Returns true if the set_str is empty.
#define set_str_isempty(t) ((t)->_size == 0)

// Returns the set_str's size.
#define set_str_size(t) ((t)->_size)

// Adds the value in order. If the value was already present, does nothing
// and returns false; otherwise inserts the new value and returns true.
// set_str takes ownership of the new value (e.g., use strdup()).
bool set_str_add(set_str* t, char* value);

// Removes and frees the given value and returns true; or does nothing
// (if the value isn't in the set_str) and returns false.
bool set_str_remove(set_str* t, const char* value);

// Returns true if value is in the set_str.
bool set_str_contains(set_str* t, const char* value);

// Returns a deep copy of the set_str.
set_str set_str_copy(const set_str* t);

// Returns true if the two set_str's have the same values.
bool set_str_equal(const set_str* t1, const set_str* t2);

// Calls the given visit function on every node in the tree in order.
// For example, given a set_str, myset:
//  void print_value(const set_str_visit_data* data) {
//      printf("%d/%d: %%s\n", data->i, data->size, data->value);
//  }
//  set_str_visit_all(&myset, print_value);
// TODO add char*context or int i to apply function?
void set_str_visit_all(const set_str* t,
                       void (*visit)(const set_str_visit_data*));

// TODO uncomment or remove
// Provides read-only access to the root if visit() isn't sufficient.
// const char** set_str_borrow_root(const set_str* t);

// Returns a new set_str that contains the values which are in t1 that are
// not in t2.
set_str set_str_difference(const set_str* t1, const set_str* t2);

// Returns a new set_str that contains the values which are in t1 or in
// t2—but not in both.
set_str set_str_symmetric_difference(const set_str* t1, const set_str* t2);

// Returns a new set_str that contains the values that t1 and t2 have in
// common.
set_str set_str_intersection(const set_str* t1, const set_str* t2);

// Returns a new set_str that contains the values from t1 and from t2 (with
// no duplicates of course).
set_str set_str_union(const set_str* t1, const set_str* t2);

// Adds every value from t2 to t1 (with no duplicates).
void set_str_unite(set_str* t1, const set_str* t2);

// Returns root for efficient iteration.
const set_str_node* set_str_root(set_str* t);

// For debugging: caller owns returned char*.
const char* set_str_tostring(const set_str* t);

// For debugging: caller owns returned char*.
const char* set_str_tostring_range(const set_str* t, SSIZE_T begin,
                                   SSIZE_T end);

// TODO set_str_isdisjoint
// TODO set_str_issubsetof
// TODO set_str_issupersetof
