// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "fx.h"
#include "cx.h"
#include "exit.h"
#include <dirent.h>
#include <err.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

inline const char* file_ext(const char* filename) {
    const char* dot = strrchr(filename, '.');
    return (!dot || dot == filename) ? "" : dot + 1;
}

char* file_change_ext(const char* filename, const char* ext) {
    char* dot = strrchr(filename, '.');
    const int dot_size = dot ? strlen(dot) : 0;
    const int ext_size = strlen(ext);
    const int base_size = strlen(filename);
    char* name = calloc(1, base_size + (ext_size - dot_size) + 1);
    assert_alloc(name);
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#pragma GCC diagnostic push
    char* p = stpncpy(name, filename, base_size - dot_size);
#pragma GCC diagnostic pop
    strcpy(p, ext);
    return name;
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

char* file_read_size(const char* filename, long long max_size, bool* ok) {
    FILE* file = fopen(filename, "rb");
    if (!file) { // failed to open
        if (ok)
            *ok = false;
        warn(NULL);
        return NULL;
    }
    bool is_ok = true;
    char* text = NULL;
    if (fseeko(file, 0, SEEK_END) == -1) // failed to seek
        goto on_error;
    long long size = ftello(file);
    if (size == -1) // failed to tell
        goto on_error;
    if (size >= max_size) { // too big
        is_ok = false;
        WARN("%s is too big to read whole: %lld >= %lld", filename, size,
             max_size);
        goto end;
    }
    if (fseeko(file, 0, SEEK_SET) == -1) // failed to seek
        goto on_error;
    text = malloc(size + 1);
    assert_alloc(text);
    long long n = fread(text, size, 1, file);
    if (n != size) {
        if (ferror(file))
            goto on_error;
        is_ok = false;
        WARN("%s failed to read entire file: %lld/%lld", filename, n, size);
    }
    goto end; // fread ok; or we've already WARN-ed
on_error:
    is_ok = false;
    warn(NULL);
end:
    if (fclose(file)) {
        is_ok = false;
        warn(NULL);
    }
    if (ok)
        *ok = is_ok;
    return text;
}

bool file_get_config_name(char* filename, const char* domain,
                          const char* appname, const char* ext) {
    struct passwd* pw = getpwuid(getuid());
    int n = snprintf(filename, FILENAME_SIZE, "%s", pw->pw_dir);
    char* end = stpcpy(filename + n, "/.config");
    if (!is_folder(filename)) {
        end = filename + n;
        *end++ = '/';
        *end++ = '.';
    } else
        *end++ = '/';
    *end = 0;
    if (domain && strlen(domain))
        sprintf(end, "%s_%s%s", domain, appname, ext);
    else
        sprintf(end, "%s%s", appname, ext);
    return is_file(filename);
}
