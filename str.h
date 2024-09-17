// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#define COMMA_I64_SIZE 28

// Returns true if char* s equals char* t
#define str_eq(s, t) (!strcmp((s), (t)))

// Returns true if char* s case-folded equals char* t
#define str_caseeq(s, t) (!strcasecmp((s), (t)))

// Returns true if char* s begins with char* prefix
#define str_begins(s, prefix) (strncmp((s), (prefix), strlen(prefix)) == 0)

// Returns true if char* s begins with char* prefix, regardless of case
#define str_casebegins(s, prefix) \
    (strncasecmp((s), (prefix), strlen(prefix)) == 0)

// Returns true if char* s ends with char* suffix
bool str_ends(const char*, const char*);

// Returns true if char* s ends with char* suffix, regardless of case
bool str_caseends(const char*, const char*);

// new means returned pointer must be free'd; _ip means changes in-place

// Modifies char*'s text in-place converting ASCII lowercase to uppercase
void str_uppercase_ip(char*);

// Returns new char* of given char*'s ASCII chars uppercased
char* str_uppercase(const char*);

// Returns new char* of up to n chars from the given char*'s ASCII chars
// uppercased
char* str_uppercasen(const char*, int n);

// Modifies char*'s text in-place converting ASCII lowercase to lowercase
void str_lowercase_ip(char*);

// Returns new char* of given char*'s ASCII chars lowercased
char* str_lowercase(const char*);

int str_strcmp(const void* s, const void* t);

int str_strcasecmp(const void* s, const void* t);

// Returns a pointer to s's first non-whitespace char or NULL if s is
// empty or all whitespace
const char* str_trim_left(const char* s);

// Like str_trim but only operates on the first n chars of s if n > 0 or
// the whole of s (i.e., up to the terminating NUL) if n == 0.
char* str_trimn(const char* s, size_t n);

// Returns a new string owned by the caller that is a copy of s with no
// leading or trailing whitespace: or NULL if s is empty or all
// whitespace. If s is nonempty and has no leading or trailing
// whitespace this function is effectively the same as strdup().
#define str_trim(s) str_trimn((s), 0)

const char* bool_to_str(bool b);

// s should be of size COMMA_I64_SIZE
void commas(char* s, int64_t n);

#define typename(x) \
    _Generic((x), \
        _Bool: "bool", \
        bool: "bool", \
        char: "char", \
        char*: "char*", \
        const char*: "const char*", \
        double: "double", \
        float: "float", \
        int: "int", \
        int*: "int*", \
        int64_t: "int64_t", \
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
        const void*: "const void*", \
        default: "unknown")
