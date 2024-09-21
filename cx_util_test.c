// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx_util_test.h"
#include "cx.h"
#include "exit.h"
#include "mx.h"
#include "str.h"
#include <stdio.h>
#include <string.h>

void check_str_eq(tinfo* tinfo, const char* s, const char* t) {
    assert_notnull(s);
    assert_notnull(t);
    tinfo->total++;
    if (strcmp(s, t) != 0) {
        WARN("FAIL: %s expected\n", tinfo->tag);
        if (strchr(s, '\n')) {
            fprintf(stderr, "==========\n%s\n----------\n%s\n==========\n",
                    s, t);
        } else
            fprintf(stderr, "\t\"%s\" !=\n\t\"%s\"\n", s, t);
    } else
        tinfo->ok++;
}

void check_casestr_eq(tinfo* tinfo, const char* s, const char* t) {
    assert_notnull(s);
    assert_notnull(t);
    tinfo->total++;
    if (strcasecmp(s, t) != 0) {
        WARN("FAIL: %s expected\n", tinfo->tag);
        if (strchr(s, '\n')) {
            fprintf(stderr, "==========\n%s\n----------\n%s\n==========\n",
                    s, t);
        } else
            fprintf(stderr, "\t\"%s\" !=\n\t\"%s\"\n", s, t);
    } else
        tinfo->ok++;
}

void check_int_eq(tinfo* tinfo, int a, int b) {
    tinfo->total++;
    if (a != b)
        WARN("FAIL: %s %d != %d\n", tinfo->tag, a, b);
    else
        tinfo->ok++;
}

void check_real_eq(tinfo* tinfo, double a, double b) {
    tinfo->total++;
    if (!is_equalish(a, b))
        WARN("FAIL: %s %g != %g\n", tinfo->tag, a, b);
    else
        tinfo->ok++;
}

void check_bool_eq(tinfo* tinfo, bool actual, bool expected) {
    tinfo->total++;
    if (actual != expected)
        WARN("FAIL: %s %s != %s\n", tinfo->tag, bool_to_str(expected),
             bool_to_str(actual));
    else
        tinfo->ok++;
}

void check_isnull(tinfo* tinfo, const void* p) {
    tinfo->total++;
    if (p)
        WARN("FAIL: %s expected NULL != %p\n", tinfo->tag, p);
    else
        tinfo->ok++;
}
