// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3
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
// If section is `NULL`, the getters and setters will used the “no
// section” (unnamed) section.
// All values are held as strings. The bool, int, and real getters parse
// the string on every call, so best to call once and keep the returned
// value.
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
// See ini_alloc().
typedef struct Ini {
    char* comment;
    VecStr sections; // index is section ID; *never* sort
    Vec items;
} Ini;

typedef struct IniItem {
    char* key;
    char* value;
    char* comment;
    int sectid;
} IniItem;

// One of these is returned by most ini_get… functions.
typedef enum IniReply {
    IniItemNotFound, // Item not found
    IniInvalidValue, // Invalid item type for getter, e.g. not an int
    IniItemFound     // Success
} IniReply;

// Creates an empty Ini.
// It must be freed with ini_free(). See also Ini.
Ini ini_alloc();

// Creates an Ini from the given `.ini` file which it parses and
// sets ok. It must be freed with ini_free(). See also Ini.
Ini ini_alloc_from_file(const char* filename, bool* ok);

// Creates an Ini with the given `.ini` file text.
// It must be freed with ini_free(). See also Ini.
Ini ini_alloc_from_str(const char* text);

// Merges the contents of the given `.ini` file and returns `true` if ok.
bool ini_merge_from_file(Ini* ini, const char* filename);

// Merges the contents of the given `.ini` file text.
void ini_merge_from_str(Ini* ini, const char* text);

// Must be called once the Ini is finished with.
// Call ini_save() first if the data is to be preserved.
void ini_free(Ini* ini);

// Saves the Ini to the given `filename` and if successful
// returns `true`.
// Not const since it sorts the items first.
bool ini_save(Ini* ini, const char* filename);

// Saves the Ini to a string which the caller owns.
// Not const since it sorts the items first.
// Provided to ease testing.
char* ini_save_to_str(Ini* ini);

// Sets the bool value for the given optional section and required key.
IniReply ini_get_bool(const Ini* ini, const char* section, const char* key,
                      bool* value);

// Sets the int value for the given optional section and required key.
IniReply ini_get_int(const Ini* ini, const char* section, const char* key,
                     int* value);

// Sets the real value for the given optional section and required key.
IniReply ini_get_real(const Ini* ini, const char* section, const char* key,
                      double* value);

// Returns the char* value for the given optional section and required key.
const char* ini_get_str(const Ini* ini, const char* section,
                        const char* key);

// Sets the bool value for the given optional sections and required key.
void ini_set_bool(Ini* ini, const char* section, const char* key,
                  bool value);

// Sets the int value for the given optional sections and required key.
void ini_set_int(Ini* ini, const char* section, const char* key, int value);

// Sets the real value for the given optional sections and required key.
void ini_set_real(Ini* ini, const char* section, const char* key,
                  double value);

// Sets the char* value for the given optional sections and required key.
void ini_set_str(Ini* ini, const char* section, const char* key,
                 const char* value);

// If both the section and the key are NULL, the comment will go at the
// start of the file. For section/key comments only use if the
// section/key exists (in which case true is returned).
// Note that only single line comments are supported.
bool ini_set_comment(Ini* ini, const char* section, const char* key,
                     const char* comment);
