// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct rbt_node rbt_node;

// A red-black tree of owned void* values.
// All data members are private; all accesses via functions.
typedef struct {
    rbt_node* _root;
    size_t _size;
    int (*_cmp)(const void*, const void*);
    void* (*_cpy)(const void*);
    void (*_destroy)(void* value);
} rbt;

typedef struct rbt_node {
    void* _value;
    rbt_node* _left;
    rbt_node* _right;
    bool _red;
} rbt_node;

typedef struct {
    int (*cmp)(const void*, const void*);
    void* (*cpy)(const void*);
    void (*destroy)(void* values);
} rbt_alloc_args;

// Allocates a new empty rbt of owned void* values.
// Caller must supply functions: cmp to compare values, cpy to copy a
// value, and destroy to free a value.
rbt rbt_alloc(rbt_alloc_args args);

// Destroys the rbt freeing its memory and also freeing every value. The rbt
// is not usable after this.
void rbt_free(rbt* t);

// Calls destroy on all the rbt's values.
void rbt_clear(rbt* t);

// Returns true if the rbt is empty.
#define rbt_isempty(t) ((t)->_size == 0)

// Returns the rbt's size.
#define rbt_size(t) ((t)->_size)

// Adds the value in order. If the value was already present, deletes the
// old value and replaces it with the new and returns false; otherwise
// inserts the new value and returns true. (This allows for the use of
// key-value values, i.e., maps, where this function matches on the key and
// replaces the value.)
// rbt takes ownership of the new value (e.g., if char* then use strdup()).
bool rbt_add(rbt* t, const void* value);

// Removes and frees the given value and returns true; or does nothing
// (if the value isn't in the rbt) and returns false.
bool rbt_remove(rbt* t, const void* value);

// Returns true and sets tree_value if value is in the rbt. The tree retains
// ownership so tree_value must not be freed.
bool rbt_find(const rbt* t, const void* value, void* tree_value);

// Returns true if value is in the rbt.
bool rbt_contains(rbt* t, const void* value);

// Returns a deep copy of the rbt including cmp, cpy, and destroy.
rbt rbt_copy(const rbt* t);

// Returns true if the two rbt's have the same values and the same cmp,
// cpy, and destroy.
bool rbt_equal(const rbt* t1, const rbt* t2);

// Applies the given apply function to every node in the tree in order.
// For example, given an rbt of Tags:
//  void print_node(const void* node) {
//      Tag *tag = node; // cast
//      printf("%s (%ld)\n", tag->name, tag->id);
//  }
//  rbt_visit(&tag_tree, print_node);
// TODO add void*context or int i to apply function?
void rbt_visit(const rbt* t, void (*apply)(const void*));

// TODO uncomment or remove
// Provides read-only access to the root if visit() isn't sufficient.
// const void** rbt_borrow_root(const rbt* t);

// Returns a new rbt that contains the values which are in t1 that are
// not in t2.
rbt rbt_difference(const rbt* t1, const rbt* t2);

// Returns a new rbt that contains the values which are in t1 or in
// t2—but not in both.
rbt rbt_symmetric_difference(const rbt* t1, const rbt* t2);

// Returns a new rbt that contains the values that t1 and t2 have in common.
rbt rbt_intersection(const rbt* t1, const rbt* t2);

// Returns a new rbt that contains the values from t1 and from t2 (with
// no duplicates of course).
rbt rbt_union(const rbt* t1, const rbt* t2);

// Adds every value from t2 to t1 (with no duplicates).
void rbt_unite(rbt* t1, const rbt* t2);

// TODO rbt_isdisjoint
// TODO rbt_issubsetof
// TODO rbt_issupersetof
