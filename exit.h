// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#define WARN(...)                                       \
    do {                                                \
        fprintf(stderr, "%s#%d: ", __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                   \
    } while (0);

#define FAIL(...)                                       \
    do {                                                \
        fprintf(stderr, "%s#%d: ", __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                   \
        exit(EXIT_FAILURE);                             \
    } while (0);

#define ARG_ERR 2
#define ARGEXIT(...)         \
    do {                     \
        printf(__VA_ARGS__); \
        exit(ARG_ERR);       \
    } while (0);

#define EXIT(...)            \
    do {                     \
        printf(__VA_ARGS__); \
        exit(EXIT_SUCCESS);  \
    } while (0);
