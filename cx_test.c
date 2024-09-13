// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "cx_util_test.h"
#include "deq_int_test.h"
#include "deq_str_test.h"
#include "fx.h"
#include "fx_test.h"
#include "ini_test.h"
#include "mx.h"
#include "mx_test.h"
#include "set_int_test.h"
#include "set_str_test.h"
#include "str.h"
#include "str_test.h"
#include "va_test.h"
#include "vec_int_test.h"
#include "vec_str_test.h"
#include "vec_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char* get_args(int argc, char** argv, bool* verbose);
void va_tests(tinfo*);

int main(int argc, char** argv) {
    bool verbose = false;
    const char* pattern = get_args(argc, argv, &verbose);
    srand((unsigned)time(NULL));
    tinfo tinfo = {"", 0, 0, verbose};
    tinfo.tag = "mx_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        mx_tests(&tinfo);
    tinfo.tag = "va_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        va_tests(&tinfo);
    tinfo.tag = "fx_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        fx_tests(&tinfo);
    tinfo.tag = "str_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        str_tests(&tinfo);
    tinfo.tag = "vec_int_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        vec_int_tests(&tinfo);
    tinfo.tag = "vec_str_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        vec_str_tests(&tinfo);
    tinfo.tag = "vec_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        vec_tests(&tinfo);
    tinfo.tag = "set_int_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        set_int_tests(&tinfo);
    tinfo.tag = "set_str_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        set_str_tests(&tinfo);
    tinfo.tag = "deq_int_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        deq_int_tests(&tinfo);
    tinfo.tag = "deq_str_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        deq_str_tests(&tinfo);
    tinfo.tag = "ini_tests";
    if (!pattern || strstr(tinfo.tag, pattern))
        ini_tests(&tinfo);
    char ok[COMMA_I64_SIZE];
    commas(ok, tinfo.ok);
    char total[COMMA_I64_SIZE];
    commas(total, tinfo.total);
    printf("%s %s/%s\n", (tinfo.ok == tinfo.total) ? "OK" : "FAIL", ok,
           total);
}

const char* get_args(int argc, char** argv, bool* verbose) {
    for (int n = 1; n < argc; ++n) {
        if (str_eq(argv[n], "-h") || str_eq(argv[n], "--help")) {
            printf("cx_test [-v|--verbose] [pattern]\n");
            exit(EXIT_SUCCESS);
        }
        if (str_eq(argv[n], "-v") || str_eq(argv[n], "--verbose")) {
            *verbose = true;
            continue;
        }
        return argv[n];
    }
    return NULL;
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
