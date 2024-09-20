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

void split_parts_dump(const SplitParts* parts) {
    if (parts->nparts)
        for (int i = 0; i < parts->nparts; ++i)
            printf("#%i«%s»\n", i, parts->parts[i]);
    else
        puts("(empty)");
}

void split_parts_free(SplitParts* parts) {
    for (int i = 0; i < parts->nparts; ++i) {
        free(parts->parts[i]);
        parts->parts[i] = NULL;
    }
    parts->nparts = 0;
}

static inline bool all_sep(const char* p, int sep) {
    for (; p && *p; ++p)
        if (sep != *p)
            return false;
    return true;
}

const char* split_err = "split_%s: more than %d parts; skipped remainder\n";

SplitParts split_chr(const char* line, int sep) {
    assert_notnull(line);
    assert_notnull(sep);
    SplitParts parts = {.nparts = 0};
    if (all_sep(line, sep)) // ∴ empty
        return parts;
    char* p = (char*)line;
    while (p) {
        char* q = strchr(p, sep);
        int size = q ? q - p : (int)strlen(p);
        char* part = parts.parts[parts.nparts++] = malloc(size + 1);
        assert_alloc(part);
        if (q) {
            strncpy(part, p, size);
            part[size] = 0;
            if (parts.nparts == MAX_SPLIT_PARTS) {
                fprintf(stderr, split_err, "chr", MAX_SPLIT_PARTS);
                break;
            }
            p = q + 1;
        } else {
            strcpy(part, p);
            break;
        }
    }
    return parts;
}

static inline const char* skip_ws(const char* p) {
    while (p && isspace(*p))
        p++;
    return p;
}

static inline const char* skip_nonws(const char* p) {
    while (p && !isspace(*p))
        p++;
    return p;
}

static inline char* make_part(const char* p, int size, bool upto_size) {
    char* q = malloc(size + 1);
    assert_alloc(q);
    if (upto_size) {
        strncpy(q, p, size);
        q[size] = 0;
    } else
        strcpy(q, p);
    return q;
}

static inline bool all_ws(const char* p) {
    for (; p && *p; ++p)
        if (!isspace(*p))
            return false;
    return true;
}

SplitParts split_ws(const char* line) {
    assert_notnull(line);
    SplitParts parts = {.nparts = 0};
    if (!*line) // empty
        return parts;
    if (all_ws(line)) // empty;
        return parts;
    const char* p = skip_ws(line); // skip leading ws
    int size = strlen(p);
    const char* end = p + size;
    while (p && *p) {
        const char* q = skip_nonws(p);
        size = q ? q - p : (int)strlen(p);
        char* part = parts.parts[parts.nparts++] = malloc(size + 1);
        assert_alloc(part);
        strncpy(part, p, size);
        part[size] = 0;
        if (q) {
            if (parts.nparts == MAX_SPLIT_PARTS) {
                fprintf(stderr, split_err, "ws", MAX_SPLIT_PARTS);
                break;
            }
            if (q > end)
                break;
            p = skip_ws(q + 1);
        } else
            break;
    }
    return parts;
}
