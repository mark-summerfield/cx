// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "vec.h"
#include "vec_str.h"
#include <stdbool.h>

typedef struct Item Item;

// One of these is returned by most ini_get functions.
typedef enum IniReply {
    IniSectionNotFound,
    IniItemNotFound,
    IniInvalidValue,
    IniItemFound
} IniReply;

// Ini represents a .ini file's sections, keys, values, and comments and
// owns all the strings it holds.
typedef struct Ini {
    char* filename;
    VecStr sections; // index is section ID
    Vec items;
} Ini;

typedef struct IniItem {
    char* key;
    char* value;
    char* comment;
    int sectid;
    char kind; // 's' | 'b' | 'i' | 'r'
    union {    // cached value if kind != 's'
        int i;
        double r;
        bool b;
    };
} IniItem;

// Creates an Ini with the given filename.
// If the file exists, loads it and sets state to IniLoaded; otherwises
// sets state to IniNotLoaded.
Ini ini_alloc(const char* filename);

// Must be called once the Ini is finished with.
// Call ini_save() first if the data is to be preserved.
void ini_free(Ini* ini);

// Saves the Ini to its given filename and if successful, sets state to
// IniSaved and returns true.
bool ini_save(Ini* ini);

// The bool, int, and real getters convert and cache on first access to
// a particular section/key pair.

IniReply ini_get_bool(Ini* ini, const char* section, const char* key,
                      bool* value);
IniReply ini_get_int(Ini* ini, const char* section, const char* key,
                     int* value);
IniReply ini_get_real(Ini* ini, const char* section, const char* key,
                      double* value);
const char* ini_get_str(Ini* ini, const char* section, const char* key);

// The bool, int, and real setters set both the value string _and_ the
// cache for each particular section/key pair.

void ini_set_bool(Ini* ini, const char* section, const char* key,
                  bool value);
void ini_set_int(Ini* ini, const char* section, const char* key, int value);
void ini_set_real(Ini* ini, const char* section, const char* key,
                  double value);
void ini_set_str(Ini* ini, const char* section, const char* key,
                 const char* value);
void ini_set_comment(Ini* ini, const char* section, const char* key,
                     const char* comment);
