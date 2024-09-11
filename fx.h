// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <stdbool.h>
#include <stddef.h>

// Returns the given filename's extension.
const char* file_extension(const char* filename);

// Returns true if the given filename or folder exists.
bool is_file_or_folder(const char* path);

// Returns true if the given filename exists.
bool is_file(const char* path);

// Returns true if the given folder exists.
bool is_folder(const char* path);

// Reads a whole file and returns it as a single string which the caller
// owns.
char* read_file(const char* filename, bool* ok);

int min(int a, int b);
int max(int a, int b);
long long minll(long long a, long long b);
long long maxll(long long a, long long b);
size_t minzu(size_t a, size_t b);
size_t maxzu(size_t a, size_t b);
double mind(double a, double b);
double maxd(double a, double b);

#define GEN_LINE(type, function) \
    type:                        \
    function

#define min(T)                                                    \
    _Generic((T), GEN_LINE(int, min), GEN_LINE(long long, minll), \
             GEN_LINE(size_t, minzu), GEN_LINE(double, mind))(T)

#define max(T)                                                    \
    _Generic((T), GEN_LINE(int, max), GEN_LINE(long long, maxll), \
             GEN_LINE(size_t, maxzu), GEN_LINE(double, maxd))(T)
