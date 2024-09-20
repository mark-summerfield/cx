// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "str.h"
#include "cx.h"
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
    char* p = (char*)s;
    assert_notnull(p);
    while (isspace(*p))
        p++;
    if (!*p)
        return NULL; // whole string is whitespace
    return p;
}

char* str_trimn(const char* s, size_t n) {
    if (!s || !*s)
        return NULL;
    char* p = (char*)s;
    assert_notnull(p);
    while (p && isspace(*p)) // trim left
        p++;
    if (p && !*p)
        return NULL; // whole string is whitespace
    char* q;
    if (n)
        q = (char*)s + n;
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

// Populates `s` with `n`'s digits plus grouping commas.
// s should be of size `COMMA_I64_SIZE`.
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

void split_parts_free(SplitParts* parts) {
    for (int i = 0; i < parts->nparts; ++i) {
        free(parts->parts[i]);
        parts->parts[i] = NULL;
    }
    parts->nparts = 0;
}

SplitParts split_chr(const char* line, int sep) {
    assert_notnull(line);
    assert_notnull(sep);
    SplitParts parts = {.nparts = 0};
    char* p = (char*)line;
    // Special case: no sep or all sep ∴ no splits ∴ copy line or empty
    bool has_sep = false;
    bool all_sep = true;
    for (char* q = p; q && *q; q++)
        if (sep == *q)
            has_sep = true;
        else
            all_sep = false;
    if (all_sep) // empty result
        return parts;
    if (!has_sep) { // copy line
        char* part = parts.parts[parts.nparts++] = malloc(strlen(line) + 1);
        assert_alloc(part);
        strcpy(part, p);
        return parts;
    }
    // Normal case: has sep
    while (p) {
        char* q = strchr(p, sep);
        if (q) {
            int size = q - p;
            char* part = parts.parts[parts.nparts] = malloc(size + 1);
            assert_alloc(part);
            strncpy(part, p, size);
            part[size] = 0;
            parts.nparts++;
            if (parts.nparts == MAX_SPLITS)
                break;
            p = q + 1;
        } else {
            int size = strlen(p);
            char* part = parts.parts[parts.nparts] = malloc(size + 1);
            assert_alloc(part);
            strcpy(part, p);
            parts.nparts++;
            break;
        }
    }
    return parts;
}

SplitParts split_ws(const char* line) {
    assert_notnull(line);
    SplitParts parts = {.nparts = 0};
    char* p = (char*)line;
    int size = strlen(p);
    // Special case: no whitespace ∴ no splits ∴ first part is copy of line
    bool ws = false;
    char* q = p;
    while (q && *q) {
        if (isspace(*q++)) {
            ws = true;
            break;
        }
    }
    if (!ws) {
        char* part = parts.parts[parts.nparts++] = malloc(size + 1);
        assert_alloc(part);
        strcpy(part, p);
        return parts;
    }
    // Normal case: whitespace
    char* end = p + size;
    while (p && isspace(*p)) // skip leading ws
        p++;
    while (p) {
        q = p + 1;
        while (q && !isspace(*q))
            q++;
        if (q >= end)
            break;
        if (q) {
            size = q - p;
            char* part = parts.parts[parts.nparts] = malloc(size + 1);
            assert_alloc(part);
            strncpy(part, p, size);
            part[size] = 0;
            parts.nparts++;
            if (parts.nparts == MAX_SPLITS)
                break;
            p = q + 1;
            while (p && isspace(*p)) // skip ws
                p++;
        } else {
            size = strlen(p);
            q = p + size;
            while (q && isspace(*q)) {
                q--;
                size--;
            }
            char* part = parts.parts[parts.nparts] = malloc(size + 1);
            assert_alloc(part);
            strcpy(part, p);
            parts.nparts++;
            break;
        }
    }
    return parts;
}
