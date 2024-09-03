// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <stdbool.h>
#include <string.h>

// Returns true if char* s equals char* t
#define str_eq(s, t) (!strcmp((s), (t)))

// Returns true if char* s case-folded equals char* t
#define str_eq_fold(s, t) (!strcasecmp((s), (t)))

// Returns true if char* s begins with char* prefix
#define str_begins(s, prefix) (strncmp((s), (prefix), strlen(prefix)) == 0)

// Returns true if char* s begins with char* prefix, regardless of case
#define str_begins_fold(s, prefix) \
    (strncasecmp((s), (prefix), strlen(prefix)) == 0)

// Returns true if char* s ends with char* suffix
bool str_ends(const char*, const char*);

// Returns true if char* s ends with char* suffix, regardless of case
bool str_ends_fold(const char*, const char*);

// new means returned pointer must be free'd; _ip means changes in-place

// Modifies char*'s text in-place converting ASCII lowercase to uppercase
void str_uppercase_ip(char*);

// Returns new char* of given char*'s ASCII chars uppercased
char* str_uppercase_alloc(const char*);

// Returns new char* of up to n chars from the given char*'s ASCII chars
// uppercased
char* str_uppercase_allocn(const char*, int n);

// Modifies char*'s text in-place converting ASCII lowercase to lowercase
void str_lowercase_ip(char*);

// Returns new char* of given char*'s ASCII chars lowercased
char* str_lowercase_alloc(const char*);

int str_strcmp(const void* s, const void* t);

int str_strcasecmp(const void* s, const void* t);

const char* bool_to_str(bool b);

/*[[noreturn]]*/ void expire(const char *fmt, ...);

#define typename(x) \
    _Generic((x), \
        _Bool: "bool", \
        char: "char", \
        char*: "char*", \
        const char*: "const char*", \
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
        const void*: "const void*", \
        default: "unknown")
