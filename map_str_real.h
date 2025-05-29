// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3
#pragma once

#include "cx.h"
#include "vec.h"
#include "vec_str.h"
#include <stdbool.h>

typedef struct MapStrRealNode MapStrRealNode;
typedef void (*visitor_fn)(const char* value, void* state);

// A red-black tree of owned or borrowed char* values.
// All data members are private; all accesses via functions.
typedef struct MapStrReal {
    MapStrRealNode* _root;
    int _size;
    Ownership _ownership;
} MapStrReal;

typedef struct MapStrRealNode {
    MapStrRealNode* left;
    MapStrRealNode* right;
    char* key;
    double value;
    bool _red;
} MapStrRealNode;

typedef struct StrRealPair {
    char* key;
    double value;
} StrRealPair;

// Allocates a new empty MapStrReal of owned or borrowed char* keys and
// double values.
MapStrReal map_str_real_alloc(Ownership ownership);

// Destroys the MapStrReal freeing its memory, and if owning, also freeing
// every char* key. The MapStrReal is not usable after this.
void map_str_real_free(MapStrReal* map);

// Calls destroy on all the MapStrReal's char* keys if owning.
void map_str_real_clear(MapStrReal* map);

// Returns Owns if the MapStrReal is owning, otherwise Borrows.
#define map_str_real_ownership(map) ((map)->_ownership)

// Adds the key-value in key order if the key isn't present and returns
// true. If the key was already present, updates the value and returns
// false.
// If owning, MapStrReal takes ownership of the new key (e.g., use
// strdup()).
bool map_str_real_set(MapStrReal* map, char* key, double value);

// Returns the given key's value and sets ok to true or returns 0.0 and
// sets ok to false if not found.
double map_str_real_get(const MapStrReal* map, const char* key, bool* ok);

// Returns true if the given key is in the map.
bool map_str_real_contains(const MapStrReal* map, const char* key);

// Removes and if owning, frees the given key-value, and returns true;
// or does nothing (if the key isn't in the MapStrReal) and returns
// false.
bool map_str_real_remove(MapStrReal* map, const char* key);

// Returns a Vec of StrRealPairs (with borrowed strings).
Vec map_str_real_to_vec(const MapStrReal* map);

// Returns a VecStr of keys (with borrowed strings).
VecStr map_str_real_keys(const MapStrReal* map);
