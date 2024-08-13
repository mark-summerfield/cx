#pragma once
// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <stdbool.h>
#include <string.h>

// Returns true if char* s's text == char* t's text
#define sx_eq(s, t) (strcmp((s), (t)) == 0)

// Returns true if char* s's text == char* t's text, regardless of case
#define sx_eq_fold(s, t) (strcasecmp((s), (t)) == 0)

// Returns true if char* s's text < char* t's text
#define sx_lt(s, t) (strcmp((s), (t)) < 0)

// Returns true if char* s's text < char* t's text, regardless of case
#define sx_lt_fold(s, t) (strcasecmp((s), (t)) < 0)

// Returns true if char* s's text <= char* t's text
#define sx_le(s, t) (strcmp((s), (t)) <= 0)

// Returns true if char* s's text <= char* t's text, regardless of case
#define sx_le_fold(s, t) (strcasecmp((s), (t)) <= 0)

// Returns true if char* s's text > char* t's text
#define sx_gt(s, t) (strcmp((s), (t)) > 0)

// Returns true if char* s's text > char* t's text, regardless of case
#define sx_gt_fold(s, t) (strcasecmp((s), (t)) > 0)

// Returns true if char* s's text >= char* t's text
#define sx_ge(s, t) (strcmp((s), (t)) >= 0)

// Returns true if char* s's text >= char* t's text, regardless of case
#define sx_ge_fold(s, t) (strcasecmp((s), (t)) >= 0)

// Returns true if char* s begins with char* prefix
#define sx_begins(s, prefix) (strncmp((s), (prefix), strlen(prefix)) == 0)

// Returns true if char* s begins with char* prefix, regardless of case
#define sx_begins_fold(s, prefix) \
    (strncasecmp((s), (prefix), strlen(prefix)) == 0)

// Returns true if char* s ends with char* suffix
bool sx_ends(const char*, const char*);

// Returns true if char* s ends with char* suffix, regardless of case
bool sx_ends_fold(const char*, const char*);

// new means returned pointer must be free'd; _ip means changes in-place

// Modifies char*'s text in-place converting ASCII lowercase to uppercase
void sx_uppercase_ip(char*);

// Returns new char* of given char*'s ASCII chars uppercased
char* sx_new_uppercase(const char*);

// Modifies char*'s text in-place converting ASCII lowercase to lowercase
void sx_lowercase_ip(char*);

// Returns new char* of given char*'s ASCII chars lowercased
char* sx_new_lowercase(const char*);

#define bool_to_str(b) ((b) ? "true" : "false")

#define typename(x) \
    _Generic((x), \
        _Bool: "bool", \
        char: "char", \
        char*: "char*", \
        double: "double", \
        float: "float", \
        int: "int", \
        int*: "int*", \
        long double: "long double", \
        long int: "long int", \
        long long int: "long long int", \
        short int: "short int", \
        signed char: "signed char", \
        unsigned char: "unsigned char", \
        unsigned int: "unsigned int", \
        unsigned long int: "unsigned long int", \
        unsigned long long int: "unsigned long long int", \
        unsigned short int: "unsigned short int", \
        void*: "void*", \
        default: "unknown")
