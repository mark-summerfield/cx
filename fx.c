// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "fx.h"
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

inline const char* file_extension(const char* filename) {
    const char* dot = strrchr(filename, '.');
    return (!dot || dot == filename) ? "" : dot + 1;
}

bool is_file_or_folder(const char* path) {
    if (!access(path, F_OK))
        return true; // its a file
    else {
        DIR* dir = opendir(path);
        if (dir) {
            closedir(dir);
            return true; // its a folder
        }
    }
    return false;
}

inline bool is_file(const char* path) { return !access(path, F_OK); }

bool is_folder(const char* path) {
    DIR* dir = opendir(path);
    if (dir) {
        closedir(dir);
        return true;
    }
    return false;
}
