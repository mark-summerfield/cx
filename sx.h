// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <stdbool.h>
#include <string.h>

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
char* sx_uppercase_alloc(const char*);

// Returns new char* of up to n chars from the given char*'s ASCII chars
// uppercased
char* sx_uppercase_allocn(const char*, int n);

// Modifies char*'s text in-place converting ASCII lowercase to lowercase
void sx_lowercase_ip(char*);

// Returns new char* of given char*'s ASCII chars lowercased
char* sx_lowercase_alloc(const char*);

int sx_strcmp(const void* s, const void* t);

#define bool_to_str(b) ((b) ? "true" : "false")

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
