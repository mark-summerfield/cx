// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_str.h"
#include "str.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static void vec_str_grow(VecStr* vec);

VecStr vec_str_alloc_custom(int cap, bool owns) {
    cap = cap > 0 ? cap : 0;
    char** values = NULL;
    if (cap) {
        values = malloc(cap * sizeof(char*));
        assert_alloc(values);
    }
    return (VecStr){
        ._size = 0, ._cap = cap, ._owns = owns, ._values = values};
}

void vec_str_free(VecStr* vec) {
    assert_notnull(vec);
    vec_str_clear(vec);
    free(vec->_values);
    vec->_values = NULL;
    vec->_cap = 0;
}

void vec_str_clear(VecStr* vec) {
    assert_notnull(vec);
    if (vec->_owns)
        for (int i = 0; i < vec->_size; ++i)
            free(vec->_values[i]);
    vec->_size = 0;
}

char* vec_str_get(const VecStr* vec, int index) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    return vec->_values[index];
}

inline char* vec_str_get_first(const VecStr* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->_values[0];
}

inline char* vec_str_get_last(const VecStr* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->_values[vec->_size - 1];
}

void vec_str_set(VecStr* vec, int index, char* value) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_notnull(value);
    assert_valid_index(vec, index);
    if (vec->_owns)
        free(vec->_values[index]);
    vec->_values[index] = value;
}

void vec_str_insert(VecStr* vec, int index, char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    assert_valid_index(vec, index);
    if (vec->_size == vec->_cap)
        vec_str_grow(vec);
    for (int i = vec->_size; i > index; --i)
        vec->_values[i] = vec->_values[i - 1];
    vec->_values[index] = value;
    vec->_size++;
}

void vec_str_add(VecStr* vec, char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    int high = vec->_size - 1;
    if (!vec->_size || strcmp(vec->_values[high], value) <= 0) {
        vec_str_push(vec,
                     value); // vec is empty -or- nonempty and value >= high
    } else {
        int low = 0;
        while (low < high) {
            int mid = (low + high) / 2;
            if (strcmp(vec->_values[mid], value) > 0)
                high = mid;
            else
                low = mid + 1;
        }
        vec_str_insert(vec, low, value);
    }
}

char* vec_str_replace(VecStr* vec, int index, char* value) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_notnull(value);
    assert_valid_index(vec, index);
    char* old = vec->_values[index];
    vec->_values[index] = value;
    return old;
}

inline void vec_str_remove(VecStr* vec, int index) {
    char* old = vec_str_take(vec, index); // vec_str_take does asserts
    if (vec->_owns)
        free(old);
}

char* vec_str_take(VecStr* vec, int index) {
    assert_notnull(vec);
    assert_nonempty(vec);
    assert_valid_index(vec, index);
    char* old = vec->_values[index];
    for (int i = index; i < vec->_size; ++i)
        vec->_values[i] = vec->_values[i + 1];
    vec->_size--;
    vec->_values[vec->_size] = NULL;
    return old;
}

char* vec_str_pop(VecStr* vec) {
    assert_notnull(vec);
    assert_nonempty(vec);
    return vec->_values[--vec->_size];
}

void vec_str_push(VecStr* vec, char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    if (vec->_size == vec->_cap)
        vec_str_grow(vec);
    vec->_values[vec->_size++] = value;
}

VecStr vec_str_copy(const VecStr* vec, bool owns) {
    assert_notnull(vec);
    VecStr out = vec_str_alloc_custom(vec->_size ? vec->_size : 0, owns);
    for (int i = 0; i < vec->_size; ++i) {
        char* value = vec->_values[i];
        vec_str_push(&out, owns ? strdup(value) : value);
    }
    return out;
}

void vec_str_merge(VecStr* vec1, VecStr* vec2) {
    assert_notnull(vec1);
    assert_notnull(vec2);
    assert(vec1->_owns == vec2->_owns &&
           "both vec_str's must be owners or both must be borrowers");
    if ((vec1->_cap - vec1->_size) <
        vec2->_size) { // vec1 doesn't have enough cap
        int cap = vec1->_size + vec2->_size;
        char** p = realloc(vec1->_values, cap * sizeof(char*));
        assert_alloc(p);
        vec1->_values = p;
        vec1->_cap = cap;
    }
    for (int i = 0; i < vec2->_size; ++i)
        vec1->_values[vec1->_size++] = vec2->_values[i]; // push
    // we do *not* free vec2's individual values even if vec2 owns since
    // their pointers are now owned by vec1
    free(vec2->_values);
    vec2->_values = NULL;
    vec2->_cap = 0;
    vec2->_size = 0;
}

bool vec_str_equal(const VecStr* vec1, const VecStr* vec2) {
    assert_notnull(vec1);
    assert_notnull(vec2);
    if (vec1->_size != vec2->_size)
        return false;
    for (int i = 0; i < vec1->_size; ++i)
        if (strcmp(vec1->_values[i], vec2->_values[i]))
            return false;
    return true;
}

int vec_str_find(const VecStr* vec, const char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    for (int i = 0; i < vec->_size; ++i)
        if (strcmp(vec->_values[i], value) == 0)
            return i;
    return VEC_NOT_FOUND;
}

int vec_str_find_last(const VecStr* vec, const char* value) {
    assert_notnull(vec);
    assert_notnull(value);
    for (int i = vec->_size - 1; i >= 0; --i)
        if (strcmp(vec->_values[i], value) == 0)
            return i;
    return VEC_NOT_FOUND;
}

void vec_str_casesort(VecStr* vec) {
    assert_notnull(vec);
    if (vec->_size)
        qsort(vec->_values, vec->_size, sizeof(char*), str_strcasecmp);
}

int vec_str_casesearch(const VecStr* vec, const char* s) {
    assert_notnull(vec);
    assert_notnull(s);
    if (vec->_size) {
        char** p = bsearch(&s, vec->_values, vec->_size, sizeof(char*),
                           str_strcasecmp);
        if (p)
            return p - vec->_values;
    }
    return VEC_NOT_FOUND;
}

void vec_str_sort(VecStr* vec) {
    assert_notnull(vec);
    if (vec->_size)
        qsort(vec->_values, vec->_size, sizeof(char*), str_strcmp);
}

int vec_str_search(const VecStr* vec, const char* s) {
    assert_notnull(vec);
    assert_notnull(s);
    if (vec->_size) {
        char** p = bsearch(&s, vec->_values, vec->_size, sizeof(char*),
                           str_strcmp);
        if (p)
            return p - vec->_values;
    }
    return VEC_NOT_FOUND;
}

char* vec_str_longest_common_prefix(const VecStr* vec) {
    if (!vec->_size)
        return NULL;
    if (vec->_size == 1)
        return strdup(vec->_values[0]);
    const char* first = vec->_values[0];
    int size = strlen(first);
    char prefix[size];
    int i = 0;
    for (; i < size; ++i) {
        char c = first[i];
        for (int j = 1; j < vec->_size; ++j) {
            char* s = vec->_values[j];
            if (((int)strlen(s)) - 1 < i || s[i] != c)
                goto end;
        }
        prefix[i] = c;
    }
end:
    if (!i)
        return NULL;
    prefix[i] = 0;
    return strdup(prefix);
}

char* vec_str_longest_common_path(const VecStr* vec) {
    if (!vec->_size)
        return NULL;
    if (vec->_size == 1)
        return strdup(vec->_values[0]);
    char* prefix = vec_str_longest_common_prefix(vec);
    if (prefix) {
        char* p = strrchr(prefix, '/');
        if (!p) { // no path separator to slice to
            free(prefix);
            return NULL;
        }
        if (p == prefix) // preserve root of /
            p++;
        *p = 0; // slice to separator
    }
    return prefix;
}

char* vec_str_join(const VecStr* vec, const char* sep) {
    assert_notnull(vec);
    const int VEC_SIZE = vec->_size;
    if (!VEC_SIZE)
        return NULL; // empty
    const int SEP_SIZE = sep ? strlen(sep) : 0;
    int size = 0;
    for (int i = 0; i < VEC_SIZE; ++i)
        size += strlen(vec->_values[i]);
    size += ((VEC_SIZE - 1) * SEP_SIZE) + 1; // +1 for 0-terminator
    char* s = malloc(size);
    assert_alloc(s);
    char* p = s;
    for (int i = 0; i < VEC_SIZE; ++i) {
        p = stpcpy(p, vec->_values[i]);
        if (sep && (i + 1 < VEC_SIZE)) // avoid adding sep at the end
            p = stpncpy(p, sep, SEP_SIZE);
    }
    return s;
}

VecStr split_str(const char* s, const char* sep) {
    assert_notnull(s);
    assert_notnull(sep);
    const int SEP_SIZE = strlen(sep);
    assert(SEP_SIZE && "can't split with empty sep");
    VecStr vec = vec_str_alloc();
    const char* p = s;
    while (p) {
        const char* q = strstr(p, sep);
        if (q) {
            vec_str_push(&vec, strndup(p, q - p));
            p = q + SEP_SIZE;
        } else {
            if (strlen(p))
                vec_str_push(&vec, strdup(p));
            break;
        }
    }
    return vec;
}

static inline bool all_sep(const char* p, int sep) {
    for (; p && *p; ++p)
        if (sep != *p)
            return false;
    return true;
}

VecStr split_chr(const char* s, int sep) {
    assert_notnull(s);
    assert_notnull(sep);
    VecStr vec = vec_str_alloc();
    if (all_sep(s, sep)) // ∴ empty
        return vec;
    const char* p = s;
    while (p) {
        const char* q = strchr(p, sep);
        int size = q ? q - p : (int)strlen(p);
        char* part = malloc(size + 1);
        assert_alloc(part);
        strncpy(part, p, size);
        part[size] = 0;
        vec_str_push(&vec, part);
        if (q)
            p = q + 1;
        else
            break;
    }
    return vec;
}

static inline bool all_ws(const char* p) {
    for (; p && *p; ++p)
        if (!isspace(*p))
            return false;
    return true;
}

VecStr split_ws(const char* s) {
    assert_notnull(s);
    VecStr vec = vec_str_alloc();
    if (!*s) // empty
        return vec;
    if (all_ws(s)) // empty;
        return vec;
    const char* p = skip_ws(s); // skip leading ws
    int size = strlen(p);
    const char* end = p + size;
    while (p && *p) {
        const char* q = skip_nonws(p);
        size = q ? q - p : (int)strlen(p);
        char* part = malloc(size + 1);
        assert_alloc(part);
        strncpy(part, p, size);
        part[size] = 0;
        vec_str_push(&vec, part);
        if (q) {
            if (q >= end)
                break;
            p = skip_ws(q + 1);
        } else
            break;
    }
    return vec;
}

void vec_str_dump(const VecStr* vec) {
    if (vec->_size)
        for (int i = 0; i < vec->_size; ++i)
            printf("[%d]«%s»\n", i, vec->_values[i]);
    else
        printf("(empty)");
}

static void vec_str_grow(VecStr* vec) {
    int cap = vec->_cap;
    assert((!cap && !vec->_values) || (cap && vec->_values));
    vec->_cap = GROW_CAP(cap);
    vec->_values = realloc(vec->_values, vec->_cap * sizeof(char*));
    assert_alloc(vec->_values);
}
