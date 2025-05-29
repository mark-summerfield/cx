// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3
#pragma once

// Prints the file and line and printf-format message to stderr.
// See also FAIL() and EXIT().
#define WARN(...)                                       \
    do {                                                \
        fprintf(stderr, "%s#%d: ", __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                   \
    } while (0)

// Prints the file and line and printf-format message to stderr and
// calls exit(EXIT_FAILURE). See also WARN() and EXIT().
#define FAIL(...)                                       \
    do {                                                \
        fprintf(stderr, "%s#%d: ", __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                   \
        exit(EXIT_FAILURE);                             \
    } while (0)

// The default return value for argument error errors.
#define ARG_ERR 2

// Prints the printf-format message to stderr and calls exit(ARG_ERR).
#define ARGEXIT(...)         \
    do {                     \
        printf(__VA_ARGS__); \
        exit(ARG_ERR);       \
    } while (0)

// Prints the printf-format message to stderr and calls exit(EXIT_SUCCESS).
// See also FAIL() and WARN().
#define EXIT(...)            \
    do {                     \
        printf(__VA_ARGS__); \
        exit(EXIT_SUCCESS);  \
    } while (0)
