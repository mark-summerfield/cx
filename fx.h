// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <stdbool.h>

// Returns the given filename's extension.
const char* file_extension(const char* filename);

// Returns true if the given filename or folder exists.
bool is_file_or_folder(const char* path);

// Returns true if the given filename exists.
bool is_file(const char* path);

// Returns true if the given folder exists.
bool is_folder(const char* path);

// Reads a whole file and returns it as a single string which the caller
// owns and sets ok if given.
char* read_file_max(const char* filename, long long max_size, bool* ok);

#define read_file(filename, ok) read_file_max((filename), 1024 * 1024, (ok))
