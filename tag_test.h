// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define MEMCHECK

typedef struct {
    char* name; // caller owns
    long id;
} Tag;

#define _TAG_BUF_SZ 20

Tag tag_make_key(char* name) {
    Tag tag = {name, 0};
    return tag;
}

// caller owns
Tag* tag_alloc(char* name, long id) {
    Tag* tag = malloc(sizeof(Tag));
    assert_alloc(tag);
    tag->name = name;
    tag->id = id;
#ifdef MEMCHECK
    static int x = 0;
    printf("tag_alloc %2d:\"%s\"#%ld\n", ++x, tag->name, tag->id);
#endif
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
    int n = sprintf(buf, "%c%c#%ld", N1, N2, ID);
    Tag* tag = tag_alloc(strndup(buf, n), ID++);
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
#ifdef MEMCHECK
    static int x = 0;
    printf("tag_free  %2d:\"%s\"#%ld\n", ++x, tag->name, tag->id);
#endif
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
void* tag_copy(const void* tag) {
    assert_notnull(tag);
    const Tag* t = tag;
    return tag_alloc(strdup(t->name), t->id);
}

// caller owns
char* tag_to_str(Tag* tag) {
    assert_notnull(tag);
    char buf[_TAG_BUF_SZ];
    int n = sprintf(buf, "\"%s\"#%ld", tag->name, tag->id);
    return strndup(buf, n);
}
