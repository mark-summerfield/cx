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

char* read_file(const char* filename, bool* ok) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        if (ok)
            *ok = false;
        warn(NULL);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* text = malloc(size + 1);
    fread(text, size, 1, file);
    if (fclose(file)) {
        if (ok)
            *ok = false;
        warn(NULL);
    }
    if (ok)
        *ok = true;
    return text;
}

inline int mini(int a, int b) { return a < b ? a : b; }

inline int maxi(int a, int b) { return a > b ? a : b; }

inline long long minll(long long a, long long b) { return a < b ? a : b; }

inline long long maxll(long long a, long long b) { return a > b ? a : b; }

inline size_t minzu(size_t a, size_t b) { return a < b ? a : b; }

inline size_t maxzu(size_t a, size_t b) { return a > b ? a : b; }

inline double mind(double a, double b) { return a < b ? a : b; }

inline double maxd(double a, double b) { return a > b ? a : b; }
