#pragma once

// Copyright © 2024 Mark Summerfield. All rights reserved.

#include <assert.h>
#include <stdbool.h>

// Use after malloc or realloc
#define assert_alloc(p) assert((p) && "failed to acquire memory")

// Use as an argument to functions that need a cmp(a, b) function
typedef int (*cmpfn)(void*, void*);

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
