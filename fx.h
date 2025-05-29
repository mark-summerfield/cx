// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3
#pragma once

#include <limits.h>
#include <stdbool.h>

// The maximum buffer size for a filename (normally 256, ∴ 255 chars)
#define FILENAME_SIZE _POSIX_PATH_MAX

// Returns true if the given filename or folder exists.
bool is_file_or_folder(const char* path);

// Returns true if the given filename exists.
bool is_file(const char* path);

// Returns true if the given folder exists.
bool is_folder(const char* path);

// Returns the given filename's extension excluding the leading dot.
const char* file_ext(const char* filename);

// Returns a copy of the given filename with the given extension which the
// caller must free.
char* file_change_ext(const char* filename, const char* ext);

// Reads a whole file (if smaller than max_size) and returns it as a
// single string which the caller owns and sets ok if not NULL.
// See also file_read().
char* file_read_size(const char* filename, long long max_size, bool* ok);

// Reads a whole file (if < 1048576 bytes) and returns it as a
// single string which the caller owns and sets ok if not NULL.
// See also file_read_size().
#define file_read(filename, ok) file_read_size(filename, 1024 * 1024, ok)

// Populates filename (of size FILENAME_SIZE) and returns true if it
// exists. The filename is either `$HOME/.config/domain_appname.ext` or
// `$HOME/.domain_appname.ext`. Domain may be NULL in which case filename
// is `$HOME/.config/appname.ext` or `$HOME/.appname.ext`.
// See also file_get_ini_name().
bool file_get_config_name(char* filename, const char* domain,
                          const char* appname, const char* ext);

// Populates filename (of size FILENAME_SIZE) and returns true if it
// exists. The filename is either `$HOME/.config/domain_appname.ini` or
// `$HOME/.domain_appname.ini`. Domain may be NULL in which case filename is
// `$HOME/.config/appname.ini` or `$HOME/.appname.ini`.
// See also file_get_config_name().
#define file_get_ini_name(filename, domain, appname) \
    file_get_config_name(filename, domain, appname, ".ini")
