// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx_util_test.h"
#include "sx_test.h"
#include "va_test.h"
#include "vec_int_test.h"
#include "vec_str_test.h"
#include <stdio.h>

void va_tests(tinfo*);

int main() {
    tinfo tinfo = {"", 0, 0};
    tinfo.tag = "va_tests";
    va_tests(&tinfo);
    tinfo.tag = "sx_tests";
    sx_tests(&tinfo);
    tinfo.tag = "vec_int_tests";
    vec_int_tests(&tinfo);
    tinfo.tag = "vec_str_tests";
    vec_str_tests(&tinfo);
    tinfo.tag = "vec_tests";
    vec_tests(&tinfo);
    printf("%s %d/%d\n", (tinfo.ok == tinfo.total) ? "OK" : "FAIL",
           tinfo.ok, tinfo.total);
}

void va_tests(tinfo* tinfo) {
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
