// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* name; // caller owns
    long id;
} Tag;

#define _TAG_BUF_SZ 20

// caller owns
Tag* tag_alloc(const char* name, long id) {
    Tag* tag = malloc(sizeof(Tag));
    tag->name = strdup(name);
    tag->id = id;
    return tag;
}

// caller owns
Tag* tag_make(bool reset) {
    static char N1 = 'A';
    static char N2 = 'a';
    static long ID = 100;
    if (reset) {
        N1 = 'A';
        N2 = 'a';
        ID = 100;
        return NULL;
    }
    char buf[_TAG_BUF_SZ];
    SSIZE_T n = sprintf(&buf[0], "%c%c#%ld", N1, N2, ID);
    Tag* tag = malloc(sizeof(Tag));
    tag->name = strndup(&buf[0], n);
    tag->id = ID++;
    N2++;
    if (N2 > 'z') {
        N2 = 'a';
        N1++;
        if (N1 > 'Z') {
            N1 = 'A';
            ID += 99;
        }
    }
    return tag;
}

void tag_free(void* t) {
    Tag* tag = t;
    free(tag->name);
    tag->name = NULL;
}

// Each void* argument is actually a pointer to a pointer, so first we
// must cast to pointer to pointer to the actual type, then we must
// dereference the outer pointer to get the inner pointer which is
// actually used.
int tag_cmp(const void* t1, const void* t2) {
    return strcmp((*(const Tag**)t1)->name, (*(const Tag**)t2)->name);
}

// caller owns
void* tag_copy(const void* old) {
    assert_notnull(old);
    const Tag* old_tag = old;
    Tag* new_tag = malloc(sizeof(Tag));
    new_tag->name = strdup(old_tag->name);
    new_tag->id = old_tag->id;
    return new_tag;
}

// caller owns
char* tag_tostring(Tag* tag) {
    assert_notnull(tag);
    char buf[_TAG_BUF_SZ];
    SSIZE_T n = sprintf(&buf[0], "\"%s\"#%ld", tag->name, tag->id);
    return strndup(&buf[0], n);
}
