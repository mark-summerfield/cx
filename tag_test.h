// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* name; // caller owns
    long id;
} Tag;

#define _TAG_BUF_SZ 20

// caller owns
Tag tag_make() {
    static char N1 = 'A';
    static char N2 = 'a';
    static char N3 = 'a';
    static long ID = 1000;
    char buf[_TAG_BUF_SZ];
    size_t n = sprintf(&buf[0], "%c%c%c#%ld", N1, N2, N3, ID);
    Tag tag = {strndup(&buf[0], n), ID};
    N3++;
    ID++;
    if (N3 > 'z') {
        N3 = 'a';
        N2++;
        if (N2 > 'z') {
            N2 = 'a';
            N1++;
            if (N1 > 'Z') {
                N1 = 'A';
                ID += 999;
            }
        }
    }
    return tag;
}

void tag_free(Tag* tag) {
    free(tag->name);
    tag->name = NULL;
}

// caller owns
char* tag_tostring(Tag* tag) {
    char buf[_TAG_BUF_SZ];
    size_t n = sprintf(&buf[0], "\"%s\"#%ld", tag->name, tag->id);
    return strndup(&buf[0], n);
}
