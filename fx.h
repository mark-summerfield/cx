// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <limits.h>
#include <stdbool.h>

#define FILENAME_SIZE _POSIX_PATH_MAX

// Returns true if the given filename or folder exists.
bool is_file_or_folder(const char* path);

// Returns true if the given filename exists.
bool is_file(const char* path);

// Returns true if the given folder exists.
bool is_folder(const char* path);

// Returns the given filename's extension excluding the leading dot.
const char* file_ext(const char* filename);

// Reads a whole file and returns it as a single string which the caller
// owns and sets ok if given.
char* file_read_size(const char* filename, long long max_size, bool* ok);

#define file_read(filename, ok) file_read_size(filename, 1024 * 1024, ok)

// Populates filename (of size FILENAME_SIZE) and returns true if it
// exists. The filename is either $HOME/.config/domain_appname.ext or
// $HOME/.domain_appname.ext. Domain may be NULL in which case filename is
// $HOME/.config/appname.ext or $HOME/.appname.ext.
bool file_get_config_name(char* filename, const char* domain,
                          const char* appname, const char* ext);

#define file_get_ini_name(filename, domain, appname) \
    file_get_config_name(filename, domain, appname, ".ini")
