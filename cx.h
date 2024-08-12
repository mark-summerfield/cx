#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <assert.h>
#include <stdbool.h>

// Use after malloc or realloc
#define assert_alloc(p) assert((p) && "failed to acquire memory")

// Use as an argument to functions that need a cmp(a, b) function
typedef int (*cmpfn)(void*, void*);

inline const char* bool_to_str(bool b) { return b ? "true" : "false"; }

#define typename(x) \
    _Generic((x), _Bool: "bool", \
    char: "char", \
    signed char: "signed char", \
    unsigned char: "unsigned char", \
    short int: "short int", \
    unsigned short int: "unsigned short int", \
    int: "int", \
    unsigned int: "unsigned int", \
    long int: "long int", \
    unsigned long int: "unsigned long int", \
    long long int: "long long int", \
    unsigned long long int: "unsigned long long int", \
    float: "float", \
    double: "double", \
    long double: "long double", \
    char *: "char*", \
    void *: "void*", \
    int *: "int*", \
    default: "unknown")
