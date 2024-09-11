// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <stdbool.h>

typedef struct {
    char* tag;
    int total;
    int ok;
    bool verbose;
} tinfo;

void check_str_eq(tinfo* tinfo, const char* s, const char* t);
void check_casestr_eq(tinfo* tinfo, const char* s, const char* t);
void check_int_eq(tinfo* tinfo, int a, int b);
void check_real_eq(tinfo* tinfo, double a, double b);
void check_bool_eq(tinfo* tinfo, bool actual, bool expected);
void check_isnull(tinfo* tinfo, const void* p);
