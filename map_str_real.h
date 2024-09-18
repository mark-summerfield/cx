// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include "vec.h"
#include <stdbool.h>

typedef struct MapStrRealNode MapStrRealNode;
typedef void (*visitor_fn)(const char* value, void* state);

// A red-black tree of owned or borrowed char* values.
// All data members are private; all accesses via functions.
typedef struct MapStrReal {
    MapStrRealNode* _root;
    int _size;
    bool _owns;
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
MapStrReal map_str_real_alloc(bool owns);

// Destroys the MapStrReal freeing its memory, and if owning, also freeing
// every char* key. The MapStrReal is not usable after this.
void map_str_real_free(MapStrReal* map);

// Calls destroy on all the MapStrReal's char* keys if owning.
void map_str_real_clear(MapStrReal* map);

// Returns true if the MapStrReal is owning.
#define map_str_real_owns(map) ((map)->_owns)

// Returns true if the MapStrReal is empty.
#define map_str_real_isempty(map) ((map)->_size == 0)

// Returns the MapStrReal's size.
#define map_str_real_size(map) ((map)->_size)

// Adds the key-value in key order. If the key was already present,
// updates the value and returns false; otherwise inserts the new key-value
// and returns true.
// If owning, MapStrReal takes ownership of the new key (e.g., use
// strdup()).
bool map_str_real_add(MapStrReal* map, char* key, double value);

// Removes and if owning, frees the given key-value, and returns true;
// or does nothing (if the key isn't in the MapStrReal) and returns
// false.
bool map_str_real_remove(MapStrReal* map, const char* key);

// Returns the given key's value and sets ok to true or returns 0.0 and
// sets ok to false if not found.
double map_str_real_get(const MapStrReal* map, const char* key, bool* ok);

// Returns a Vec of StrRealPairs.
Vec map_str_real_to_vec(const MapStrReal* map);
