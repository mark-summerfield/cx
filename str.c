// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "str.h"
#include "cx.h"
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Each void* argument is actually a pointer to a pointer, so first we
// must cast to pointer to pointer to the actual type, then we must
// dereference the outer pointer to get the inner pointer which is
// actually used.
int str_strcmp(const void* s, const void* t) {
    return strcmp(*(const char**)s, *(const char**)t);
}

int str_strcasecmp(const void* s, const void* t) {
    return strcasecmp(*(const char**)s, *(const char**)t);
}

bool str_ends(const char* s, const char* suffix) {
    size_t size = strlen(s);
    if (size < strlen(suffix))
        return false;
    return strcmp(s + size - strlen(suffix), suffix) == 0;
}

bool str_ends_fold(const char* s, const char* suffix) {
    size_t size = strlen(s);
    if (size < strlen(suffix))
        return false;
    return strcasecmp(s + size - strlen(suffix), suffix) == 0;
}

void str_uppercase_ip(char* s) {
    assert(s && "can't uppercase NULL");
    while (*s) {
        *s = toupper(*s);
        s++;
    }
}

char* str_uppercase(const char* s) {
    assert(s && "can't uppercase NULL");
    char* u = malloc(strlen(s) + 1);
    assert_alloc(u);
    int i = 0;
    while (*s) {
        u[i++] = toupper(*s);
        s++;
    }
    u[i] = 0;
    return u;
}

char* str_uppercasen(const char* s, int n) {
    assert(s && "can't uppercase NULL");
    char* u = malloc(n + 1); // +1 for terminating 0
    assert_alloc(u);
    int i = 0;
    while (*s && i < n) {
        u[i++] = toupper(*s);
        s++;
    }
    u[i] = 0;
    return u;
}

void str_lowercase_ip(char* s) {
    assert(s && "can't lowercase NULL");
    while (*s) {
        *s = tolower(*s);
        s++;
    }
}

char* str_lowercase(const char* s) {
    assert(s && "can't lowercase NULL");
    char* u = malloc(strlen(s) + 1);
    assert_alloc(u);
    int i = 0;
    while (*s) {
        u[i++] = tolower(*s);
        s++;
    }
    u[i] = 0;
    return u;
}

const char* str_trim_left(const char* s) {
    char* p = (char*)s;
    while (isspace(*p))
        p++;
    return p;
}

inline const char* bool_to_str(bool b) { return b ? "true" : "false"; }
