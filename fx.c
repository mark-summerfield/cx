// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "fx.h"
#include "cx.h"
#include <dirent.h>
#include <err.h>
#include <pwd.h>
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
        warn("%s is too big to read whole: %lld >= %lld", filename, size,
             max_size);
        goto end;
    }
    if (fseeko(file, 0, SEEK_SET) == -1) // failed to seek
        goto on_error;
    text = malloc(size + 1);
    assert_alloc(text);
    (void)fread(text, size, 1, file);
    if (!ferror(file)) // fread ok; else fallthrough to on_error
        goto end;
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
    int n = snprintf(filename, FILENAME_MAX, "%s", pw->pw_dir);
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
