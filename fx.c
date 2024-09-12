// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "fx.h"
#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
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

char* read_file_max(const char* filename, long long max_size, bool* ok) {
    FILE* file = fopen(filename, "rb");
    if (!file) { // failed to open
        if (ok)
            *ok = false;
        warn(NULL);
        return NULL;
    }
    char* text = NULL;
    bool is_ok = true;
    if (fseek(file, 0, SEEK_END) == -1) { // failed to seek
        is_ok = false;
        warn(NULL);
        goto end;
    }
    long long size = ftell(file); // failed to tell
    if (size == -1) {
        is_ok = false;
        warn(NULL);
        goto end;
    }
    if (size >= max_size) { // too big
        is_ok = false;
        warn("%s is too big to read whole (%lld)", filename, size);
        goto end;
    }
    if (fseek(file, 0, SEEK_SET) == -1) { // failed to seek
        is_ok = false;
        warn(NULL);
        goto end;
    }
    text = malloc(size + 1);
    fread(text, size, 1, file);
    if (ferror(file)) { // failed to read
        is_ok = false;
        warn(NULL);
    }
end:
    if (fclose(file)) {
        is_ok = false;
        warn(NULL);
    }
    if (ok)
        *ok = is_ok;
    return text;
}
