// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vec_str.h"
#include <stdbool.h>

typedef struct SetStrNode SetStrNode;
typedef void (*visitor_fn)(const char* value, void* state);

// A red-black tree of owned or borrowed char* values.
// All data members are private; all accesses via functions.
typedef struct SetStr {
    SetStrNode* _root;
    int _size;
    bool _owns;
} SetStr;

typedef struct SetStrNode {
    SetStrNode* left;
    SetStrNode* right;
    char* value;
    bool _red;
} SetStrNode;

// Allocates a new empty SetStr of owned or borrowed char* values.
SetStr set_str_alloc(bool owns);

// Destroys the SetStr freeing its memory, and if owning, also freeing every
// value. The SetStr is not usable after this.
void set_str_free(SetStr* set);

// Calls destroy on all the SetStr's values if owning.
void set_str_clear(SetStr* set);

// Returns true if the SetStr is owning.
#define set_str_owns(set) ((set)->_owns)

// Returns true if the SetStr is empty.
#define set_str_isempty(set) ((set)->_size == 0)

// Returns the SetStr's size.
#define set_str_size(set) ((set)->_size)

// Adds the value in order. If the value was already present, does nothing
// and returns false; otherwise inserts the new value and returns true.
// If owning, SetStr takes ownership of the new value (e.g., use strdup()).
bool set_str_add(SetStr* set, char* value);

// Removes and if owning, frees the given value, and returns true; or does
// nothing (if the value isn't in the SetStr) and returns false.
bool set_str_remove(SetStr* set, const char* value);

// Returns true if value is in the SetStr.
bool set_str_contains(const SetStr* set, const char* value);

// Returns a copy (deep if owns is true) of the SetStr which the caller now
// owns.
SetStr set_str_copy(const SetStr* set, bool owns);

// Returns true if the two SetStr's have the same values.
bool set_str_equal(const SetStr* set1, const SetStr* set2);

// Returns a new SetStr that contains the values which are in set1 that are
// not in set2. If owns the result is an owned copy.
SetStr set_str_difference(const SetStr* set1, const SetStr* set2,
                          bool owns);

// Returns a new SetStr that contains the values that set1 and set2 have in
// common. If owns the result is an owned copy.
SetStr set_str_intersection(const SetStr* set1, const SetStr* set2,
                            bool owns);

// Returns a new SetStr that contains the values from set1 and from set2
// (with no duplicates of course). If owns the result is an owned copy.
SetStr set_str_union(const SetStr* set1, const SetStr* set2, bool owns);

// Adds every value from set2 to set1 (with no duplicates).
// If set1 owns, set2's values are deep copied.
void set_str_unite(SetStr* set1, const SetStr* set2);

// Returns an owned or borrowed VecStr which the caller owns with all
// the set's values in order.
// See the source of this function and its helper to see how to iterate a
// set.
VecStr set_str_to_vec(const SetStr* set, bool owns);

// Returns the set as a caller-owned string of sep-separated strings.
char* set_str_join(const SetStr* set, const char* sep);

// Calls visitor(value, state) on every element of the set.
// See the tests for an example of use.
void set_str_visit(const SetStr* set, void* state, visitor_fn);

// Used for tests to verify balance.
int set_str_max_depth(const SetStr* set);
