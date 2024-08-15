// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

struct Tag {
    char* name; // caller owns
    long id;
};

// caller owns
Tag tag_make() {
    static char N1 = 'A';
    static char N2 = 'a';
    static char N3 = 'a';
    static long ID = 1000;
    Tag tag = {sprintf("%c%c%c#%ld", N1, N2, N3++), ID++};
    if (N3 > 'z') {
        N3 = 'a';
        N2++;
        if (N2 > 'z') {
            N2 = 'a';
            N1++;
            if (N1 > 'Z') {
                N1 = 'A';
                ID += 1000;
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
    return sprintf("\"%s\"#%ld", tag->name, tag->id);
}
