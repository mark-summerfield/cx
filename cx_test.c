// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx_util_test.h"
#include "set_int_test.h"
#include "set_str_test.h"
#include "str_test.h"
#include "va_test.h"
#include "vec_int_test.h"
#include "vec_str_test.h"
#include "vec_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void va_tests(tinfo*);

#define UNUSED(x) (void)(x)

int main(int argc, char** argv) {
    UNUSED(argv);
    srand((unsigned)time(NULL));
    tinfo tinfo = {"", 0, 0, argc > 1};
    tinfo.tag = "va_tests";
    va_tests(&tinfo);
    tinfo.tag = "str_tests";
    str_tests(&tinfo);
    tinfo.tag = "vec_int_tests";
    vec_int_tests(&tinfo);
    tinfo.tag = "vec_str_tests";
    vec_str_tests(&tinfo);
    tinfo.tag = "vec_tests";
    vec_tests(&tinfo);
    tinfo.tag = "set_int_tests";
    set_int_tests(&tinfo);
    tinfo.tag = "set_str_tests";
    set_str_tests(&tinfo);
    printf("%s %d/%d\n", (tinfo.ok == tinfo.total) ? "OK" : "FAIL",
           tinfo.ok, tinfo.total);
}

void va_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    tinfo->total++;
    int i = va_test(5);
    if (i != 8) {
        fprintf(stderr, "FAIL: %s expected 8 go %d\n", tinfo->tag, i);
    } else
        tinfo->ok++;
    tinfo->total++;
    i = va_test(2, 9);
    if (i != 11) {
        fprintf(stderr, "FAIL: %s expected 11 go %d\n", tinfo->tag, i);
    } else
        tinfo->ok++;
}
