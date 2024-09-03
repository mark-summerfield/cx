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
