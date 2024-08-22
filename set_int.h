// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vec_int.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct SetIntNode SetIntNode;

// A red-black tree of owns int values.
// All data members are private; all accesses via functions.
typedef struct {
    SetIntNode* _root;
    int _size;
} SetInt;

typedef struct SetIntNode {
    SetIntNode* left;
    SetIntNode* right;
    int value;
    bool _red;
} SetIntNode;

// Allocates a new empty SetInt of owns int values.
SetInt set_int_alloc();

// Destroys the SetInt freeing its memory and also freeing every value. The
// SetInt is not usable after this.
void set_int_free(SetInt* set);

// Calls destroy on all the SetInt's values.
void set_int_clear(SetInt* set);

// Returns true if the SetInt is empty.
#define set_int_isempty(set) ((set)->_size == 0)

// Returns the SetInt's size.
#define set_int_size(set) ((set)->_size)

// Adds the value in order. If the value was already present, does nothing
// and returns false; otherwise inserts the new value and returns true.
bool set_int_add(SetInt* set, int value);

// Removes the given value and returns true; or does nothing
// (if the value isn'set in the SetInt) and returns false.
bool set_int_remove(SetInt* set, int value);

// Returns true if value is in the SetInt.
bool set_int_contains(const SetInt* set, int value);

// Returns a deep copy of the SetInt which the caller now owns.
SetInt set_int_copy(const SetInt* set);

// Returns true if the two SetInt's have the same values.
bool set_int_equal(const SetInt* set1, const SetInt* set2);

// Returns a new SetInt that contains the values which are in set1 that are
// not in set2.
SetInt set_int_difference(const SetInt* set1, const SetInt* set2);

// Returns a new SetInt that contains the values that set1 and set2 have in
// common.
SetInt set_int_intersection(const SetInt* set1, const SetInt* set2);

// Returns a new SetInt that contains the values from set1 and from set2
// (with no duplicates of course).
SetInt set_int_union(const SetInt* set1, const SetInt* set2);

// Adds every value from set2 to set1 (with no duplicates).
void set_int_unite(SetInt* set1, const SetInt* set2);

// Returns an VecInt which the caller owns and that has a copy of all the
// set's values in order.
// See the source of this function and its helper to see how to iterate a
// set.
VecInt set_int_to_vec(const SetInt* set);

// Returns the set as a caller-owns string of space-separated ints.
char* set_int_to_str(const SetInt* set);

// Used for tests to verify balance.
int set_int_max_depth(const SetInt* set);

// TODO set_int_symmetric_difference
// TODO set_int_isdisjoint
// TODO set_int_issubsetof
// TODO set_int_issupersetof
