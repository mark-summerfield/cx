// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "vec.h"
#include "vec_str.h"
#include <stdbool.h>

#define INI_NO_SECTION -91919

// Ini represents a `.ini` file's sections, keys, values, and comments and
// owns all the strings it holds.
// Case is preserved but in all getters and setters section and key
// names are case-insensitive, and in all setters, section and key names
// and values are trimmed.
// Comments are discarded on reading, but if set, are written on writing.
// The easiest way to use `Ini` is as follows:
// ```
// // create with all sections & key-values with their default values
// Ini ini = ini_alloc_from_str();
// ini_set_comment(...); // add any comments wanted in the saved file
//     :
// // if an existing .ini exists on disk, merge in its values
// if (is_file(filename))
//     ini_merge_from_file(&ini, filename);
// ... // use the ini
// ini_free(&ini); // finish
// ```
// See @link ini_alloc@.
typedef struct Ini {
    char* comment;
    VecStr sections; // index is section ID; *never* sort
    Vec items;
} Ini;

// @private@
typedef struct IniItem {
    char* key;
    char* value;
    char* comment;
    int sectid;
} IniItem;

// One of these is returned by most ini_get... functions.
typedef enum IniReply {
    IniItemNotFound, // Item not found
    IniInvalidValue, // Invalid item type for getter, e.g. not an int
    IniItemFound     // Success
} IniReply;

Ini ini_alloc();

Ini ini_alloc_from_file(const char* filename, bool* ok);

Ini ini_alloc_from_str(const char* text);

bool ini_merge_from_file(Ini* ini, const char* filename);

void ini_merge_from_str(Ini* ini, const char* text);

void ini_free(Ini* ini);

bool ini_save(Ini* ini, const char* filename);

char* ini_save_to_str(Ini* ini);

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
