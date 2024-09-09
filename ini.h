// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "vec.h"
#include "vec_str.h"
#include <stdbool.h>

#define INI_UNNAMED_SECTION "!"

typedef struct Item Item;

// One of these is returned by most ini_get functions.
typedef enum IniReply {
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
} IniItem;

// Creates an Ini with the given filename and if the file exists, loads it.
Ini ini_alloc(const char* filename);

// Creates an Ini with the given filename and with the given .ini file
// text. Provided to ease testing.
Ini ini_alloc_from_str(const char* filename, const char* text);

// Must be called once the Ini is finished with.
// Call ini_save() first if the data is to be preserved.
void ini_free(Ini* ini);

// Saves the Ini to its given filename and if successful returns true.
// Not const since it sorts the items first.
bool ini_save(Ini* ini);

// Saves the Ini to a string which the caller owns.
// Not const since it sorts the items first.
// Provided to ease testing.
char* ini_save_to_str(Ini* ini);

// Not const since it sorts the items first.
// Provided to ease testing.
bool ini_equal(Ini* ini1, Ini* ini2);

IniReply ini_get_bool(const Ini* ini, const char* section, const char* key,
                      bool* value);
IniReply ini_get_int(const Ini* ini, const char* section, const char* key,
                     int* value);
IniReply ini_get_real(const Ini* ini, const char* section, const char* key,
                      double* value);
const char* ini_get_str(const Ini* ini, const char* section,
                        const char* key);

void ini_set_bool(Ini* ini, const char* section, const char* key,
                  bool value);
void ini_set_int(Ini* ini, const char* section, const char* key, int value);
void ini_set_real(Ini* ini, const char* section, const char* key,
                  double value);
void ini_set_str(Ini* ini, const char* section, const char* key,
                 const char* value);
bool ini_set_comment(Ini* ini, const char* section, const char* key,
                     const char* comment);
