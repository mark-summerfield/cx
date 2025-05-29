// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3

#include "str.h"
#include "cx.h"
#include "exit.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

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

bool str_caseends(const char* s, const char* suffix) {
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
    if (!s || !*s)
        return NULL;
    const char* p = skip_ws(s);
    if (!*p)
        return NULL; // whole string is whitespace
    return p;
}

char* str_trimn(const char* s, size_t n) {
    if (!s || !*s)
        return NULL;
    const char* p = skip_ws(s);
    if (p && !*p)
        return NULL; // whole string is whitespace
    const char* q;
    if (n)
        q = s + n;
    else {
        q = p;
        assert_notnull(q);
        while (*q) // find \0
            q++;
        q--; // last char
    }
    assert_notnull(q);
    while (isspace(*q)) // trim right
        q--;
    if (q < p)
        return NULL; // all whitespace
    size_t size = q - p + 1;
    char* r = malloc(size + 1); // allow for \0
    assert_alloc(r);
    strncpy(r, p, size);
    r[size] = 0;
    return r;
}

inline const char* bool_to_str(bool b) { return b ? "true" : "false"; }

void commas(char* s, int64_t n) {
    assert_notnull(s);
    char raw[COMMA_I64_SIZE] = "";
    char commabuf[COMMA_I64_SIZE] = "";
    int bare_char_count = snprintf(raw, COMMA_I64_SIZE, "%" PRId64, n);
    int digits = 0;
    int j = 0;
    for (int i = bare_char_count - 1; i >= 0; --i) {
        digits++;
        commabuf[j++] = raw[i];
        if (digits == 3) {
            commabuf[j++] = ',';
            digits = 0;
        }
    }
    char* p = commabuf;
    char* q = commabuf + j - 1;
    while (p < q) {
        char c = *q;
        *q-- = *p;
        *p++ = c;
    }
    p = commabuf;
    if (*p == '-' && *(p + 1) == ',') {
        p++;
        *p = '-';
    } else if (*p == ',')
        p++;
    strncpy(s, p, j);
    s[j] = 0;
}

inline const char* skip_ws(const char* p) {
    while (p && isspace(*p))
        p++;
    return p;
}

inline const char* skip_nonws(const char* p) {
    while (p && !isspace(*p))
        p++;
    return p;
}
