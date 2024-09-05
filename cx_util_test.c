// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx_util_test.h"
#include "cx.h"
#include "str.h"
#include <stdio.h>
#include <string.h>

void check_str_eq(tinfo* tinfo, const char* s, const char* t) {
    tinfo->total++;
    if (strcmp(s, t) != 0) {
        fprintf(stderr, "FAIL: %s expected\n\t\"%s\" !=\n\t\"%s\"\n",
                tinfo->tag, s, t);
    } else
        tinfo->ok++;
}

void check_int_eq(tinfo* tinfo, int a, int b) {
    tinfo->total++;
    if (a != b) {
        fprintf(stderr, "FAIL: %s expected %d != %d\n", tinfo->tag, a, b);
    } else
        tinfo->ok++;
}

void check_bool_eq(tinfo* tinfo, bool actual, bool expected) {
    tinfo->total++;
    if (actual != expected)
        fprintf(stderr, "FAIL: %s expected %s != %s\n", tinfo->tag,
                bool_to_str(expected), bool_to_str(actual));
    else
        tinfo->ok++;
}

void check_isnull(tinfo* tinfo, const void* p) {
    tinfo->total++;
    if (p)
        fprintf(stderr, "FAIL: %s expected NULL != %p\n", tinfo->tag, p);
    else
        tinfo->ok++;
}
